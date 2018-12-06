/*\
 * powerup.c - all the powerups
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

#include "render.h"
#include "intersect.h"
#include "game.h"
#include "powerup.h"


#define POWERUP_VEL_X    0.05f //how fast they move
#define POWERUP_ANIM_ROT 4.0f  //how fast it rotates


//spawns a new powerup
void SpawnPowerup(POWERUP * p)
{
   if(p)
   {
      if(rand() & 1)
      {
         p->pos.x = 0.0f;
         p->vel.x = POWERUP_VEL_X;
      }
      else
      {
         p->pos.x = 40.0f;
         p->vel.x = -(POWERUP_VEL_X);
      }
      p->pos.y = (float)(rand() % 30);
      p->vel.y = (float)rand() / ((float)RAND_MAX * 4.0f) - 0.125f;

      p->type = rand() % NUM_POWERUPS;

      p->anim_rot = 0;
      p->rot_axis.x = (float)rand() / (float)RAND_MAX;
      p->rot_axis.y = (float)rand() / (float)RAND_MAX;
      p->rot_axis.z = (float)rand() / (float)RAND_MAX;

      p->clip_circle.radius = 0.8f;
   }
}

//updates it
void UpdatePowerup(POWERUP * p)
{
   if(p)
   {
      p->pos.x += p->vel.x;
      p->pos.y += p->vel.y;

      if(p->pos.y < 0.0f)
         p->pos.y += 30.0f;
      else if(p->pos.y >= 30.0f)
         p->pos.y -= 30.0f;

      if((p->anim_rot += POWERUP_ANIM_ROT) >= 360.0f)
         p->anim_rot -= 360.0f;

      p->region = GetRegion(&p->pos, 0.8f);

      p->clip_circle.center.x = p->pos.x;
      p->clip_circle.center.y = p->pos.y;
   }
}

//draws it
void DrawPowerup(POWERUP * p)
{
   if(p)
   {
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();

      glTranslatef(p->pos.x, p->pos.y, 0.0f);
      glRotatef(p->anim_rot, p->rot_axis.x, p->rot_axis.y, p->rot_axis.z);
      if(p->type == POW_TYPE_1UP)
         glColor3f(0.25f, 1.0f, 0.25f);
      else if(p->type == POW_TYPE_HITALL)
         glColor3f(0.25f, 0.25f, 1.0f);
      else if(p->type == POW_TYPE_SHIELD)
         glColor3f(0.75f, 0.75f, 0.75f);
      else if(p->type == POW_TYPE_RAPIDFIRE)
         glColor3f(1.0f, 0.1f, 0.1f);
      else
         glColor3f(0.1f, 0.1f, 0.1f);

      glutWireOctahedron();

      glPopMatrix();
   }
}
