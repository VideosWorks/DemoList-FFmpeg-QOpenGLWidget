/*\
 * alien.c - code for the alien object
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

#include <stdlib.h>
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


#define ALIEN_VEL_X     0.05f         //x velocity for alien
#define ALIEN_BIG_VEL_Y 0.03f         //y velocity for big alien
#define ALIEN_SM_VEL_Y  0.05f         //y velocity for small alien
#define ALIEN_ANIM_ROT  4.0f          //how fast alien rotates (for animation)
#define ALIEN_SM_ROT    6.0f          //how fast BIG alien rotates
#define ALIEN_SHOT_T    (SHOT_LIFE * 2) //how many frames between alien shots
#define ALIEN_SM_SHOT_T SHOT_LIFE     //for small alien

char * taunts[] =
{
   "ALL YOUR BASE ARE BELONG TO US",
   "YOU ARE ON THE WAY TO DESTRUCTION",
   "FOR GREAT JUSTICE",
   "YOU HAVE NO CHANCE TO SURVIVE MAKE YOUR TIME",
   "HA HA HA HA",
   "HOW ARE YOU GENTLEMEN!!",
   "TAKE OFF EVERY 'ZIG'!!",
   "MOVE 'ZIG'",
   "SOMEBODY SET UP US THE BOMB",
   "IT'S YOU!!",
   "YOU KNOW WHAT YOU DOING",
   "WHAT YOU SAY!!",
};
#define NUM_TAUNTS 12


MODEL g_alien_model; //the alien ship model


//fills the alien model
void FillAlienModel(void)
{
   g_alien_model = MakeAlienShip();
}

//spawns the alien into the game world
void SpawnAlien(ALIEN * a, int big)
{
   if(a)
   {
      if(rand() & 1)
      {
         a->pos.x = 0.0f;
         a->vel.x = ALIEN_VEL_X;
      }
      else
      {
         a->pos.x = 40.0f;
         a->vel.x = -(ALIEN_VEL_X);
      }
      a->pos.y = (float)(rand() % 30);
      a->vel.y = (big ? ALIEN_BIG_VEL_Y : ALIEN_SM_VEL_Y) * ((rand() & 1) ? -1.0f : 1.0f);

      a->scale = (big ? 4.0f : 2.0f);

      a->time_to_shoot = 0;

      a->anim_rot = 0.0f;
   }
}

//updates stuff for the alien
void UpdateAlien(ALIEN * a, const PLAYER * player, const LIST * asteroid_list)
{
   const LIST * ast_cur;
   int chance, cuttoff1, cuttoff2;
   float angle = 0.0f;

   if(a)
   {
      a->pos.x += a->vel.x;
      a->pos.y += a->vel.y;

      if(a->pos.y < 0.0f)
         a->pos.y += 30.0f;
      else if(a->pos.y >= 30.0f)
         a->pos.y -= 30.0f;

      if(!(rand() % 1500))
         NewBurst(&a->pos, taunts[rand() % NUM_TAUNTS]);

      a->region = GetRegion(&a->pos, a->scale * 0.5f);

      if(a->scale == 2.0f)
      {
         SetPoly(&a->clip_poly, 0.0f, -1.0f, -0.866f, 0.5f, 0.866f, 0.5f, &a->pos, a->anim_rot);
/*
         SetPoly(&a->clip_polys[0], 0.0f, 0.0f,  0.0f,   -1.0f,  0.174f,  0.1f, &a->pos, a->anim_rot);
         SetPoly(&a->clip_polys[1], 0.0f, 0.0f, -0.866f,  0.5f,  0.0f,   -0.2f, &a->pos, a->anim_rot);
         SetPoly(&a->clip_polys[2], 0.0f, 0.0f,  0.866f,  0.5f, -0.174f,  0.1f, &a->pos, a->anim_rot);
*/
      }
      else
      {
         SetPoly(&a->clip_poly, 0.0f, -2.0f, -1.732f, 1.0f, 1.732f, 1.0f, &a->pos, a->anim_rot);
/*
         SetPoly(&a->clip_polys[0], 0.0f, 0.0f,  0.0f,   -2.0f,  0.348f,  0.2f, &a->pos, a->anim_rot);
         SetPoly(&a->clip_polys[1], 0.0f, 0.0f, -1.732f,  1.0f,  0.0f,   -0.4f, &a->pos, a->anim_rot);
         SetPoly(&a->clip_polys[2], 0.0f, 0.0f,  1.732f,  1.0f, -0.348f,  0.2f, &a->pos, a->anim_rot);
*/
      }

      if(!(rand() % 200))
         a->vel.y = -a->vel.y;

      if(a->time_to_shoot)
         a->time_to_shoot--;
      else
      {
         a->time_to_shoot = (a->scale == 2.0f ? ALIEN_SM_SHOT_T : ALIEN_SHOT_T);

         chance = rand() % 10;

         if(a->scale == 2.0f)
         {
            cuttoff1 = 1;
            cuttoff2 = 6;
         }
         else
         {
            cuttoff1 = 5;
            cuttoff2 = 7;
         }

         if(chance >= cuttoff2 && asteroid_list)
         {
            for(ast_cur = asteroid_list; ast_cur; ast_cur = ast_cur->next)
            {
               angle = (float)atan2(((ASTEROID *)ast_cur->d)->pos.y - a->pos.y, ((ASTEROID *)ast_cur->d)->pos.x - a->pos.x) / DEG_TO_RAD;
               break;
            }
         }
         else if(chance >= cuttoff1 && player)
            angle = (float)atan2(player->pos.y - a->pos.y, player->pos.x - a->pos.x) / DEG_TO_RAD;
         else
            angle = (float)(rand() % 360);

         NewShot(&a->pos, &a->vel, angle, 0);
      }

      if((a->anim_rot -= (a->scale == 2.0f ? ALIEN_SM_ROT : ALIEN_ANIM_ROT)) < 0.0f)
         a->anim_rot += 360.0f;
   }
}

//draws the alien object
void DrawAlien(ALIEN * a)
{
   if(a)
   {
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glTranslatef(a->pos.x, a->pos.y, 0.0f);
      glRotatef(a->anim_rot, 0.0f, 0.0f, 1.0f);
      glScalef(a->scale, a->scale, a->scale);
      DrawModel(g_alien_model);
      glPopMatrix();
   }
}
