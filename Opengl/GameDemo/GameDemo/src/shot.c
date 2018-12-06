/*\
 * shot.c - code for the shot object
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
#include "shot.h"


#define SHOT_SPEED 0.3f //how fast they move


//spawns a new shot into the world
void SpawnShot(SHOT * s, const VECTOR2 * pos, const VECTOR2 * vel, float angle, int player_owned)
{
   if(s && pos && vel)
   {
      s->life = SHOT_LIFE;
      s->pos.x = pos->x;
      s->pos.y = pos->y;
      s->vel.x = vel->x + (float)cos(angle * DEG_TO_RAD) * SHOT_SPEED;
      s->vel.y = vel->y + (float)sin(angle * DEG_TO_RAD) * SHOT_SPEED;

      s->player_owned = player_owned;
   }
}

//updates a shot in the world
void UpdateShot(SHOT * s)
{
   if(s)
   {
      //add to position
      s->pos.x += s->vel.x;
      s->pos.y += s->vel.y;
      WrapVector(&s->pos);
      s->region = GetRegion(&s->pos, 0.0f);
      s->life--;
   }
}

//draws a dot where the shot is
void DrawShot(SHOT * s)
{
   if(s)
   {
      glDisable(GL_LIGHTING);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glTranslatef(s->pos.x, s->pos.y, 0.0f);
      glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
      glutSolidSphere(0.07f, 4.0f, 2.0f);
      glPopMatrix();
      glEnable(GL_LIGHTING);
   }
}
