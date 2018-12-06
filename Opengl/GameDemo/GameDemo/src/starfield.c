/*\
 * starfield.c - code to manage the starfield
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
#include <math.h>

#include "render.h"
#include "starfield.h"


#define STARFIELD_ROT_SPEED 0.05f


//fills the starfield with data
void InitStarfield(STARFIELD * s)
{
   int i;
   float range;

   if(s)
   {
      s->rot = 0;

      for(i = 0; i < NUM_STARS; i++)
      {
         s->stars[i].x = (float)rand() / (float)(RAND_MAX / 2) - 1.0f; //x = [-1, 1]
         range = 2.0f * (float)sqrt(1.0f - s->stars[i].x*s->stars[i].x);
         s->stars[i].y = (float)rand() / (float)(RAND_MAX) * range - range * 0.5f; //y = inside unit circle

         s->alpha[i] = (float)rand() / ((float)RAND_MAX * 4.0f / 3.0f) + 0.25f; //alpha = [.25, 1]
      }
   }
}

//updates the burst
void UpdateStarfield(STARFIELD * s)
{
   if(s)
   {
      if((s->rot += STARFIELD_ROT_SPEED) > 360.0f)
         s->rot -= 360.0f;
   }
}

//draws the burst
void DrawStarfield(STARFIELD * s)
{
   int i;

   if(s)
   {
      glDisable(GL_LIGHTING);

      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glRotatef(s->rot, 0.0f, 0.0f, -1.0f);

      glBegin(GL_POINTS);
      for(i = 0; i < NUM_STARS; i++)
      {
         glColor4f(s->alpha[i], s->alpha[i], s->alpha[i], 1.0f);
         glVertex3f(s->stars[i].x * 50.0f, s->stars[i].y * 50.0f, -50.0f);
      }
      glEnd();

      glPopMatrix();

      glEnable(GL_LIGHTING);
   }
}
