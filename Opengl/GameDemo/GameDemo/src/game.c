/*\
 * game.c - a basic game engine
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>
#include <math.h>

#include "render.h"
#include "geometry.h"
#include "game.h"
#include "intersect.h"
#include "list.h"
#include "asteroid.h"
#include "player.h"
#include "shot.h"
#include "alien.h"
#include "explosion.h"
#include "burst.h"
#include "starfield.h"
#include "powerup.h"
#include "hiscore.h"
#include "sound.h"


#define RESET_TIME 150           //frames before the game resets after the last asteroid is destroyed
#define HISCT_DELAY 75           //frames after play death before it displays hi score table


void Score(int, const VECTOR2 *);//adds to the score
void Clipping(void);             //performs clipping each frame

void PlayerHit(void);            //destroys the player
void AsteroidHit(LIST *, int);   //destroys the asteroid, spawns new ones if necessary
void AlienHit(LIST *, int);      //destroys the alien
void PowerupHit(LIST *, int);    //destroys the powerup without getting it
void GetPowerup(LIST *);         //collects the powerup and destroys it


int      g_paused         = 0;    //whether the game is paused

PLAYER   g_player;                //the player object
LIST   * g_asteroid_list  = NULL; //list of asteroids
LIST   * g_alien_list     = NULL; //list of aliens
LIST   * g_shot_list      = NULL; //the shots
LIST   * g_explosion_list = NULL; //list of explosions
LIST   * g_burst_list     = NULL; //list of text bursts
LIST   * g_powerup_list   = NULL; //list of powerups
STARFIELD g_starfield;            //starfield object

HISCORE_TABLE g_hisct;            //hiscore table
unsigned int  g_hisct_delay = 0;  //delay to drawing hisct

unsigned int g_keys = 0;           //an integer whose bits tell which keys are up and down

unsigned int g_frame = 0;          //a frame counter
unsigned int g_time_to_reset = 0;  //counter that is set to delay between the last asteroid and the game resetting

unsigned int g_level = 0;          //what level we're on
unsigned int g_score = 0;          //player's score
unsigned int g_ships = 0;          //number of ships player has left

float        g_fps = 0.0f;         //fps the game's getting


//toggles between paused and unpaused
void TogglePause(void)
{
   if((g_paused = !g_paused) != 0)
      glutPostRedisplay();
}

//called once to set up game
void InitGame(void)
{
   memset(&g_player, 0, sizeof(g_player));
   memset(&g_starfield, 0, sizeof(g_starfield));
   memset(&g_hisct, 0, sizeof(g_hisct));

   ReadHiScoreTable(&g_hisct);

   InitGeometry();
   FillAsteroidModel();
   FillPlayerModel();
   FillAlienModel();

   InitStarfield(&g_starfield);
}

//called one time; sets up game objects
void NewGame(void)
{
   if(g_hisct_delay)
      return;

   g_keys = 0;
   g_level = 0;
   g_score = 0;
   g_ships = 3;

   NextLevel();
}

//initializes or resets the state of the game
void NextLevel(void)
{
   int i;

   PlaySoundIndex(SND_NEWLEVEL);

   g_level++;

   InitStarfield(&g_starfield);

   DeleteEntireList(&g_asteroid_list);
   DeleteEntireList(&g_alien_list);
   DeleteEntireList(&g_shot_list);
   DeleteEntireList(&g_explosion_list);
   DeleteEntireList(&g_burst_list);
   DeleteEntireList(&g_powerup_list);

   i = g_player.powerups;
   ResetPlayer(&g_player);
   g_player.powerups = i;

   for(i = 0; i < (int)(log((double)g_level) + 2.5); i++)
      SpawnAsteroid(NewListNode(&g_asteroid_list, sizeof(ASTEROID)), NULL, 4.0f);

   Score((g_level > 100 ? 999 : (g_level - 1) * 10), NULL); //completion bonus

   g_frame = 0;
   g_time_to_reset = 0;
}

//updates the world one tick
void UpdateGame(void)
{
   int numbigasteroids = 0, nummedasteroids = 0;
   LIST * cur;
   LIST * next;
   static unsigned int fpsframes = 0;
   static unsigned int last_fps_time = 0;
   unsigned int t;
   char hiscore_name[HS_NAME_LENGTH];

   if(g_paused)
      return;

   UpdateStarfield(&g_starfield);

   for(cur = g_asteroid_list; cur; cur = cur->next)
   {
      if(((ASTEROID *)cur->d)->scale == 4.0f)
         numbigasteroids++;
      else if(((ASTEROID *)cur->d)->scale == 2.0f)
         nummedasteroids++;
   }

   t = 1500*numbigasteroids + 500*nummedasteroids + (int)(16384.0/pow((double)g_score, 0.3010299956639811952));
   if(g_ships
      && !g_time_to_reset
      && g_level > 2
      && !(rand() % t))
   {
      NewAlien(g_score >= 10000 ? (rand() % 3) : 1);
   }

   t = (g_ships + 1) * 1000;
   if(g_ships
      && g_ships < 11
      && !g_time_to_reset
      && !(rand() % t))
   {
      NewPowerup();
   }

   for(cur = g_asteroid_list; cur; cur = cur->next)
      UpdateAsteroid(cur->d);

   if(g_ships)
      UpdatePlayer(&g_player, g_keys);

   for(cur = g_alien_list; cur; cur = next)
   {
      next = cur->next;

      UpdateAlien(cur->d, (g_ships ? &g_player : NULL), g_asteroid_list);
      if(((ALIEN *)cur->d)->pos.x < 0.0f || ((ALIEN *)cur->d)->pos.x > 40.0f)
         RemoveNode(cur, &g_alien_list);
   }

   for(cur = g_shot_list; cur; cur = next)
   {
      next = cur->next;

      UpdateShot(cur->d);
      if(!(((SHOT *)cur->d)->life))
         RemoveNode(cur, &g_shot_list);
   }

   for(cur = g_explosion_list; cur; cur = next)
   {
      next = cur->next;

      UpdateExplosion(cur->d);
      if(!(((EXPLOSION *)cur->d)->life))
         RemoveNode(cur, &g_explosion_list);
   }

   for(cur = g_burst_list; cur; cur = next)
   {
      next = cur->next;

      UpdateBurst(cur->d);
      if(!(((BURST *)cur->d)->life))
         RemoveNode(cur, &g_burst_list);
   }

   for(cur = g_powerup_list; cur; cur = next)
   {
      next = cur->next;

      UpdatePowerup(cur->d);
      if(((POWERUP *)cur->d)->pos.x < 0.0f || ((POWERUP *)cur->d)->pos.x > 40.0f)
         RemoveNode(cur, &g_powerup_list);
   }

   Clipping();

   g_frame++; //next frame
   if(g_time_to_reset && !(--g_time_to_reset))
      NextLevel();

   if(!g_ships && g_hisct_delay && !(--g_hisct_delay) && IsScoreHi(&g_hisct, g_score) && GetHiScoreName(hiscore_name))
      AddHiScore(&g_hisct, hiscore_name, g_score, g_level);

   fpsframes++;
   t = glutGet(GLUT_ELAPSED_TIME);
   if(t >= last_fps_time + 1000)
   {
      g_fps = (float)fpsframes / (float)(t - last_fps_time) * 1000.0f;
      fpsframes = 0;
      last_fps_time = t;
   }

   glutPostRedisplay();
}

//renders one frame
void Render(void)
{
   LIST * cur;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   DrawStarfield(&g_starfield);

   for(cur = g_asteroid_list; cur; cur = cur->next)
      DrawAsteroid(cur->d);

   if(g_ships)
      DrawPlayer(&g_player);
   else
   {
      RenderText(15.2858f, 27.0f, 50.0f, 1.0f, 0.75f, 0.0f, 0.0f, "GAME OVER");

      if(!g_hisct_delay)
      {
         RenderText(13.1906f, 26.0f, 50.0f, 0.5f, 0.75f, 0.5f, 0.5f, "hit F2 to start a new game");
         DrawHiScoreTable(&g_hisct);
      }
   }

   for(cur = g_alien_list; cur; cur = cur->next)
      DrawAlien(cur->d);

   for(cur = g_shot_list; cur; cur = cur->next)
      DrawShot(cur->d);

   for(cur = g_explosion_list; cur; cur = cur->next)
      DrawExplosion(cur->d);

   for(cur = g_burst_list; cur; cur = cur->next)
      DrawBurst(cur->d);

   for(cur = g_powerup_list; cur; cur = cur->next)
      DrawPowerup(cur->d);

   if(g_paused)
   {
      RenderText(0.011792f, 6.0f, 50.0f, 6.36f, 1.0f, 0.5f, 0.5f, "paused");
      RenderText(11.0954f, 4.0f, 50.0f, 1.0f, 1.0f, 0.5f, 0.5f, "(hit p to resume)");
   }

   RenderText(29.3f, 28.7f, 50.0f, 1.0f, 0.75f, 0.75f, 0.75f, "%10d", g_score);
   RenderText(0.1f, 28.7f, 50.0f, 1.0f, 0.75f, 0.75f, 0.75f, "%d", (g_ships ? g_ships - 1 : 0));
   if(g_level < 1000)
   {
      if(g_time_to_reset)
         RenderText(7.9526f, 0.5f, 50.0f, 1.0f, 0.25f, 0.25f, 0.25f, "warping to level %03d...", g_level + 1);
      else if(g_level)
         RenderText(15.2858f, 0.5f, 50.0f, 1.0f, 0.25f, 0.25f, 0.25f, "level %03d", g_level);
   }
   else
      RenderText(13.1906f, 0.5f, 50.0f, 1.0f, 1.0f, 1.0f, 1.0f, "you are a god");

   if(g_ships && g_time_to_reset)
   {
      RenderText(15.2858f, 15.0f, 50.0f, 1.0f, 0.0f, 0.75f, 0.0f, "GOOD JOB!");
      RenderText(14.5001f, 14.0f, 50.0f, 0.5f, 0.5f, 0.75f, 0.5f, "completion bonus: %3d", (g_level > 99 ? 999 : g_level * 10));
   }

   RenderText(0.1f, 0.2f, 0.0f, 0.5f, 0.5f, 0.0f, 0.5f, "%.1ffps", g_fps);

   glFlush();
   glutSwapBuffers();
}

//gets a region from a position and size
unsigned int GetRegion(const VECTOR2 * v, float radius)
{
   unsigned int column = 0;
   unsigned int row    = 0;
   float x_sub_rad, x_add_rad, y_sub_rad, y_add_rad;

   if(v)
   {
      x_sub_rad = v->x - radius;
      x_add_rad = v->x + radius;
      y_sub_rad = v->y - radius;
      y_add_rad = v->y + radius;

      if(x_add_rad < 10.0f)
         column = 0x111;
      else if(x_sub_rad < 10.0f)
         column = 0x333;
      else if(x_add_rad < 20.0f)
         column = 0x222;
      else if(x_sub_rad < 20.0f)
         column = 0x666;
      else if(x_add_rad < 30.0f)
         column = 0x444;
      else if(x_sub_rad < 30.0f)
         column = 0xccc;
      else
         column = 0x888;

      if(y_add_rad < 10.0f)
         row = 0x00f;
      else if(y_sub_rad < 10.0f)
         row = 0x0ff;
      else if(y_add_rad < 20.0f)
         row = 0x0f0;
      else if(y_sub_rad < 20.0f)
         row = 0xff0;
      else
         row = 0xf00;
   }

   return (column & row);
}

//adds a shot to the list of shots
void NewShot(const VECTOR2 * pos, const VECTOR2 * vel, float angle, int player_owned)
{
   PlaySoundIndex(SND_SPAWN_SHOT);

   SpawnShot(NewListNode(&g_shot_list, sizeof(SHOT)), pos, vel, angle, player_owned);
}

//adds an alien to the list of aliens
void NewAlien(int big)
{
   PlaySoundIndex(SND_SPAWN_ALIEN);

   SpawnAlien(NewListNode(&g_alien_list, sizeof(ALIEN)), big);
}

void NewPowerup(void)
{
   PlaySoundIndex(SND_SPAWN_POWERUP);

   SpawnPowerup(NewListNode(&g_powerup_list, sizeof(POWERUP)));
}

//spawns a text burst
void NewBurst(const VECTOR2 * pos, const char * msg, ...)
{
   char buf[1024];
   va_list vList;

   va_start(vList, msg);
   vsprintf(buf, msg, vList);
   va_end(vList);

   SpawnBurst(NewListNode(&g_burst_list, sizeof(BURST)), pos, buf);
}

//destroys all active asteroids
void DestroyAllAsteroids(int from_player)
{
   LIST * cur;
   LIST * next;

   for(cur = g_asteroid_list; cur; cur = next) //works because new asteroids are added before head
   {
      next = cur->next;
      AsteroidHit(cur, from_player);
   }
}

void ProcKey(int down, int mask)
{
   if(down)
      g_keys |= mask;
   else
      g_keys &= ~mask;
}

//adds to the player's score
void Score(int add, const VECTOR2 * burst_pos)
{
   int floorbefore = g_score / 10000; //integer divide

   if(burst_pos)
      NewBurst(burst_pos, "%d", add);
   g_score += add;

   if(floorbefore != g_score / 10000) //if floor before != floor after, we've gone over the boundary
   {
      g_ships++;
      NewBurst(&g_player.pos, "one-up");
   }
}

//performs clipping each frame
void Clipping(void)
{
   LIST * cur1;
   LIST * cur2;
   LIST * next;

   for(cur1 = g_asteroid_list; cur1; cur1 = next)
   {
      next = cur1->next;

      if(g_ships
         && !g_player.wait_time
         && (((ASTEROID *)cur1->d)->region & g_player.region)
         && PolyInCircle(&g_player.clip_poly, &(((ASTEROID *)cur1->d)->clip_circle))) //if the player hits the asteroid
      {
         PlayerHit();
         AsteroidHit(cur1, 0);
         continue;
      }

      for(cur2 = g_alien_list; cur2; cur2 = cur2->next)
      {
         if((((ASTEROID *)cur1->d)->region & ((ALIEN *)cur2->d)->region)
            && (PolyInCircle(&(((ALIEN *)cur2->d)->clip_poly/*s[0]*/), &(((ASTEROID *)cur1->d)->clip_circle))
               /*|| PolyInCircle(&(((ALIEN *)cur2->d)->clip_polys[1]), &(((ASTEROID *)cur1->d)->clip_circle))
               || PolyInCircle(&(((ALIEN *)cur2->d)->clip_polys[2]), &(((ASTEROID *)cur1->d)->clip_circle))*/))
         {
            AlienHit(cur2, 0);
            AsteroidHit(cur1, 0);
            break;
         }
      }
      if(cur2) //if we broke early from the loop
         continue;

      for(cur2 = g_shot_list; cur2; cur2 = cur2->next)
      {
         if((((ASTEROID *)cur1->d)->region & ((SHOT *)cur2->d)->region)
            && PointInCircle(&(((SHOT *)cur2->d)->pos), &(((ASTEROID *)cur1->d)->clip_circle))) //if any shot hits the asteroid
         {
            AsteroidHit(cur1, ((SHOT *)cur2->d)->player_owned);
            RemoveNode(cur2, &g_shot_list);
            break;
         }
      }
      if(cur2) //if we broke early from the loop
         continue;
   }

   for(cur1 = g_shot_list; cur1; cur1 = next)
   {
      next = cur1->next;

      if(!((SHOT *)cur1->d)->player_owned
         && g_ships
         && !g_player.wait_time
         && (g_player.region & ((SHOT *)cur1->d)->region)
         && PointInPoly(&(((SHOT *)cur1->d)->pos), &g_player.clip_poly)) //if the alien shot is inside player
      {
         RemoveNode(cur1, &g_shot_list);
         PlayerHit();
         continue;
      }

      if(((SHOT *)cur1->d)->player_owned)
      {
         for(cur2 = g_alien_list; cur2; cur2 = cur2->next)
         {
            if((((SHOT *)cur1->d)->region & ((ALIEN *)cur2->d)->region)
               && (PointInPoly(&(((SHOT *)cur1->d)->pos), &(((ALIEN *)cur2->d)->clip_poly/*s[0]*/))
                  /*|| PointInPoly(&(((SHOT *)cur1->d)->pos), &(((ALIEN *)cur2->d)->clip_polys[1]))
                  || PointInPoly(&(((SHOT *)cur1->d)->pos), &(((ALIEN *)cur2->d)->clip_polys[2]))*/))
            {
               RemoveNode(cur1, &g_shot_list);
               AlienHit(cur2, 1);
               break;
            }
         }
         if(cur2)
            continue;
      }

      for(cur2 = g_powerup_list; cur2; cur2 = cur2->next)
      {
         if((((SHOT *)cur1->d)->region & ((POWERUP *)cur2->d)->region)
            && PointInCircle(&(((SHOT *)cur1->d)->pos), &(((POWERUP *)cur2->d)->clip_circle)))
         {
            PowerupHit(cur2, ((SHOT *)cur1->d)->player_owned);
            RemoveNode(cur1, &g_shot_list);
            break;
         }
      }
      if(cur2)
         continue;
   }

   if(g_ships
      && !g_player.wait_time)
   {
      for(cur1 = g_alien_list; cur1; cur1 = cur1->next)
      {
         if((g_player.region & ((ALIEN *)cur1->d)->region)
            && (PolyInPoly(&g_player.clip_poly, &(((ALIEN *)cur1->d)->clip_poly/*s[0]*/))
               /*|| PolyInPoly(&g_player.clip_poly, &(((ALIEN *)cur1->d)->clip_polys[1]))
               || PolyInPoly(&g_player.clip_poly, &(((ALIEN *)cur1->d)->clip_polys[2]))*/))
         {
            PlayerHit();
            AlienHit(cur1, 0);
            break;
         }
      }
   }

   if(g_ships)
   {
      for(cur1 = g_powerup_list; cur1; cur1 = next)
      {
         next = cur1->next;

         if((g_player.region & ((POWERUP *)cur1->d)->region)
            && PolyInCircle(&g_player.clip_poly, &(((POWERUP *)cur1->d)->clip_circle)))
         {
            GetPowerup(cur1);
            continue;
         }
      }
   }
}

//destroys the player, respawns it
void PlayerHit(void)
{
   if(g_player.powerups & PL_POW_SHIELD)
   {
      g_player.powerups &= (~PL_POW_SHIELD);
      g_player.wait_time = PLAYER_SPAWN_WAIT;
   }
   else
   {
      PlaySoundIndex(SND_EXP_SHIP);

      SpawnExplosion(NewListNode(&g_explosion_list, sizeof(EXPLOSION)), &g_player.pos, NUM_EXPLOSION_PARTICLES);

      ResetPlayer(&g_player);
      if(g_ships && !(--g_ships))
         g_hisct_delay = HISCT_DELAY;
   }
}

//destroys the asteroid, spawning new ones if necessary
void AsteroidHit(LIST * a, int fromplayershot)
{
   if(a)
   {
      PlaySoundIndex(SND_EXP_ASTEROID);

      SpawnExplosion(NewListNode(&g_explosion_list, sizeof(EXPLOSION)), &(((ASTEROID *)a->d)->pos), (int)(((ASTEROID *)a->d)->scale) * 8);

      if(((ASTEROID *)a->d)->scale > 1.0f)
      {
         if(fromplayershot)
            Score(((ASTEROID *)a->d)->scale > 2.0f ? 20 : 50, &(((ASTEROID *)a->d)->pos));

         SpawnAsteroid(NewListNode(&g_asteroid_list, sizeof(ASTEROID)), &(((ASTEROID *)a->d)->pos), ((ASTEROID *)a->d)->scale * 0.5f);
         SpawnAsteroid(NewListNode(&g_asteroid_list, sizeof(ASTEROID)), &(((ASTEROID *)a->d)->pos), ((ASTEROID *)a->d)->scale * 0.5f);
      }
      else if(fromplayershot)
         Score(100, &(((ASTEROID *)a->d)->pos));

      RemoveNode(a, &g_asteroid_list);

      if(!g_asteroid_list && g_ships)
         g_time_to_reset = RESET_TIME;
   }
}

//destroys the alien ship
void AlienHit(LIST * a, int fromplayershot)
{
   if(a)
   {
      PlaySoundIndex(SND_EXP_OTHER);

      SpawnExplosion(NewListNode(&g_explosion_list, sizeof(EXPLOSION)), &(((ALIEN *)a->d)->pos), (((ALIEN *)a->d)->scale == 2.0f ? NUM_EXPLOSION_PARTICLES / 2 : NUM_EXPLOSION_PARTICLES));

      if(fromplayershot)
         Score((((ALIEN *)a->d)->scale == 2.0f ? 500 : 200), &(((ALIEN *)a->d)->pos));

      RemoveNode(a, &g_alien_list);
   }
}

//destroys the powerup without getting it
void PowerupHit(LIST * p, int fromplayershot)
{
   if(p)
   {
      PlaySoundIndex(SND_EXP_OTHER);

      SpawnExplosion(NewListNode(&g_explosion_list, sizeof(EXPLOSION)), &(((POWERUP *)p->d)->pos), NUM_EXPLOSION_PARTICLES / 2);

      if(fromplayershot)
         Score(666, &(((POWERUP *)p->d)->pos));

      RemoveNode(p, &g_powerup_list);
   }
}

//collects the powerup and destroys it
void GetPowerup(LIST * p)
{
   char * msg = NULL;

   if(p)
   {
      switch(((POWERUP *)p->d)->type)
      {
         case POW_TYPE_1UP:
            msg = "one-up";
            g_ships++;
            break;

         case POW_TYPE_HITALL:
            msg = "hit-all";
            DestroyAllAsteroids(1);
            break;

         case POW_TYPE_SHIELD:
            msg = "shield";
            g_player.powerups |= PL_POW_SHIELD;
            break;

         case POW_TYPE_RAPIDFIRE:
            msg = "rapid-fire";
            g_player.powerups |= PL_POW_RAPIDFIRE;
            break;
      }

      PlaySoundIndex(SND_GET_POWERUP);

      NewBurst(&(((POWERUP *)p->d)->pos), msg);

      RemoveNode(p, &g_powerup_list);
   }
}
