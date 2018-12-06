/*\
 * explosion.c - code for the explosion object
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
#include "explosion.h"


//spawns a new explosion at a given location
void SpawnExplosion(EXPLOSION * e, const VECTOR2 * pos, int size)
{
   int i;

   if(e && pos)
   {
      if(size > NUM_EXPLOSION_PARTICLES)
         size = NUM_EXPLOSION_PARTICLES;

      e->life = (rand() % 25) + 26; //26-50

      for(i = 0; i < size; i++)
      {
         e->pos[i].x = pos->x;
         e->pos[i].y = pos->y;

         e->vel[i].x = (float)rand() / ((float)RAND_MAX * 2.0f) - 0.25f;
         e->vel[i].y = (float)rand() / ((float)RAND_MAX * 2.0f) - 0.25f;

         e->fade_time[i] = (rand() % (e->life - 15)) + 16;
      }
   }
}

//updates the explosion
void UpdateExplosion(EXPLOSION * e)
{
   int i;

   if(e)
   {
      for(i = 0; i < NUM_EXPLOSION_PARTICLES; i++)
      {
         e->pos[i].x += e->vel[i].x; //doesn't wrap
         e->pos[i].y += e->vel[i].y;

         if(e->fade_time[i])
            e->fade_time[i]--;
      }

      e->life--;
   }
}

//draws the explosion
void DrawExplosion(EXPLOSION * e)
{
   float fade;
   int i;

   if(e)
   {
      glDisable(GL_LIGHTING);
      glMatrixMode(GL_MODELVIEW);
      for(i = 0; i < NUM_EXPLOSION_PARTICLES; i++)
      {
         if(e->fade_time[i])
         {
            fade = (float)e->fade_time[i] / 50.0f;

            glPushMatrix();
            glTranslatef(e->pos[i].x, e->pos[i].y, 0.0f);
            glColor4f((0.4f > fade ? 0.4f : fade), fade, 0.0f, 1.0f);
            glutSolidSphere(0.07f + fade / 10.0f, 5.0f, 2.0f);
            glPopMatrix();
         }
      }
      glEnable(GL_LIGHTING);
   }
}
