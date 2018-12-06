/*\
 * asteroid.c - code for the asteroid object
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#include "config.h"

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
#include "asteroid.h"
#include "main.h"


#ifdef CFG_RES_INSTALL_PATH //possibly defined from config.h, if it was available
#define TEXTURE_PRE CFG_RES_INSTALL_PATH "/gfx/" //if it was defined, append gfx to directory name
#else
#define TEXTURE_PRE "gfx/" //if it wasn't defined, hope gfx/ is in current directory
#endif


MODEL   g_asteroid_model; //the asteroid model
//TEXTURE g_asteroid_tex;   //asteroid texture


//fills the asteroid model
void FillAsteroidModel(void)
{
   TEXTURE tex = CreateTexture(TEXTURE_PRE "asteroid.bmp");
   if(!tex)
      ast_warn("Failed to load texture: " TEXTURE_PRE "asteroid.bmp" "\nThe asteroids will look dumb, but you can still play.\n");

   g_asteroid_model = MakeAsteroid(tex);
}

//makes a new asteroid object
void SpawnAsteroid(ASTEROID * a, const VECTOR2 * pos, float scale)
{
   if(a)
   {
      if(pos) //if they give a position
      {
         a->pos.x = pos->x + (float)rand() / (float)RAND_MAX - 0.5f; //use it, with slight random perturbation
         a->pos.y = pos->y + (float)rand() / (float)RAND_MAX - 0.5f;
      }
      else //or, we make one up
      {
         if(rand() & 1)
         {
            a->pos.x = 0.0f;
            a->pos.y = (float)(rand() % 30);
         }
         else
         {
            a->pos.x = (float)(rand() % 40);
            a->pos.y = 0.0f;
         }
      }

      a->vel.x = (float)rand() / ((float)RAND_MAX * 4.0f) - 0.125f;
      a->vel.y = (float)rand() / ((float)RAND_MAX * 4.0f) - 0.125f;

      a->rot     = (float)(rand() % 360);
      a->rot_vel = (float)rand() / (float)(RAND_MAX / 4);

      a->rot_axis.x = (float)rand() / (float)RAND_MAX;
      a->rot_axis.y = (float)rand() / (float)RAND_MAX;
      a->rot_axis.z = (float)rand() / (float)RAND_MAX;

      a->scale = scale;

      a->clip_circle.radius = 0.5 * scale;
   }
}

//updates an asteroid
void UpdateAsteroid(ASTEROID * a)
{
   if(a)
   {
      a->pos.x += a->vel.x;
      a->pos.y += a->vel.y;
      WrapVector(&a->pos);
      a->region = GetRegion(&a->pos, 0.5f * a->scale);

      a->clip_circle.center.x = a->pos.x;
      a->clip_circle.center.y = a->pos.y;

      if((a->rot += a->rot_vel) >= 360.0f)
         a->rot -= 360.0f;
   }
}

//draws an asteroid
void DrawAsteroid(ASTEROID * a)
{
   if(a)
   {
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glTranslatef(a->pos.x, a->pos.y, 0.0f);
      glRotatef(a->rot, a->rot_axis.x, a->rot_axis.y, a->rot_axis.z);
      glScalef(a->scale, a->scale, a->scale);
      //SetTexture(g_asteroid_tex);
      DrawModel(g_asteroid_model);
      //SetTexture(0);
      glPopMatrix();
   }
}
