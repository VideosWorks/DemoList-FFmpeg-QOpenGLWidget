/*\
 * player.c - code for the player object
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
#include "player.h"
#include "shot.h"


#define ACCEL_FACTOR    0.007f        //added to player's velocity to accelerate
#define MAX_VELOCITY    0.3f          //max velocity a player can attain
#define DAMPING_FACTOR  0.0005f       //decrease velocity by this much
#define ROT_SPEED       5.0f          //how fast they turn left/right
#define PLAYER_ANIM_ROT 3.0f          //how fast the player rotates (for animation)

#define NUM_PLAYER_SHOTS 4            //how many simultaneous shots
#define NEXT_SHOOT_TIME  (SHOT_LIFE / NUM_PLAYER_SHOTS) //how many frames between shots
#define NEXT_SHOOT_TIME_RF (NEXT_SHOOT_TIME / 2)


MODEL g_player_model; //the player ship model

extern unsigned int g_frame; //from game.c


//fills the player model
void FillPlayerModel(void)
{
   g_player_model = MakePlayerShip();
}

//makes a new player object
void ResetPlayer(PLAYER * p)
{
   if(p)
   {
      memset(p, 0, sizeof(*p));

      p->pos.x = 20.0f;
      p->pos.y = 15.0f;
      p->rot   = 90.0f;
      p->wait_time = PLAYER_SPAWN_WAIT;
   }
}

//updates a player object
void UpdatePlayer(PLAYER * p, unsigned int keys)
{
   float mag;

   if(p)
   {
      if(p->wait_time) //if we're waiting
         p->wait_time--; //update wait timer
      else
      {
         //shoot stuff
         if(p->time_to_shoot) //if we can't shoot yet
            p->time_to_shoot--; //update shot timer
         else
         {
            if(keys & KEY_MASK_SHOOT) //if we're TRYING to shoot
            {
               p->time_to_shoot =
                  ((p->powerups & PL_POW_RAPIDFIRE)
                  ? NEXT_SHOOT_TIME_RF
                  : NEXT_SHOOT_TIME);
               NewShot(&p->pos, &p->vel, p->rot, 1);
            }
         }
      }

      //accelerate (add to velocity)
      if(keys & KEY_MASK_ACCEL)
      {
         p->vel.x += (float)cos(p->rot * DEG_TO_RAD) * ACCEL_FACTOR;
         p->vel.y += (float)sin(p->rot * DEG_TO_RAD) * ACCEL_FACTOR;
      }

      //get magnitude of velocity
      mag = (float)sqrt(p->vel.x*p->vel.x + p->vel.y*p->vel.y);
      //keep max velocity under MAX_VELOCITY
      if(mag > MAX_VELOCITY) //if it goes beyond, clamp to MAX_VELOCITY
      {
         p->vel.x *= MAX_VELOCITY / mag;
         p->vel.y *= MAX_VELOCITY / mag;
         mag = MAX_VELOCITY;
      }

      //add velocity to position
      p->pos.x += p->vel.x;
      p->pos.y += p->vel.y;
      WrapVector(&p->pos);
      p->region = GetRegion(&p->pos, 0.5f);

      SetPoly(&p->clip_poly, 0.5f, 0.0f, -0.4f, 0.26f, -0.4f, -0.26f, &p->pos, p->rot);

      //dampen velocity
      if(mag > 0.0f)
      {
         p->vel.x -= p->vel.x / mag * DAMPING_FACTOR;
         p->vel.y -= p->vel.y / mag * DAMPING_FACTOR;
      }

      //rotate left
      if(keys & KEY_MASK_LEFT)
      {
         if((p->rot += ROT_SPEED) >= 360.0f)
            p->rot -= 360.0f;
      }
      //rotate right
      if(keys & KEY_MASK_RIGHT)
      {
         if((p->rot -= ROT_SPEED) < 0.0f)
            p->rot += 360.0f;
      }

      //update animation rotation
      if((p->anim_rot += PLAYER_ANIM_ROT) >= 360.0f)
         p->anim_rot -= 360.0f;
   }
}

//draws a player
void DrawPlayer(PLAYER * p)
{
   if(p && (!p->wait_time || (g_frame & 8))) //draw only if not waiting, or if we ARE waiting, toggled every 8th frame
   {
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glTranslatef(p->pos.x, p->pos.y, 0.0f);
      glRotatef(p->rot, 0.0f, 0.0f, 1.0f);
      glRotatef(p->anim_rot, 1.0f, 0.0f, 0.0f);
      DrawModel(g_player_model);
      if(p->powerups & PL_POW_SHIELD)
      {
         glColor3f(0.3f, 0.3f, 0.3f);
         glScalef(0.4f, 0.4f, 0.4f);
         glutWireDodecahedron();
      }
      glPopMatrix();
   }
}
