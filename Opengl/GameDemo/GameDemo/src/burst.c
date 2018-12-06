/*\
 * burst.c - code for bursts of text in-game
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#include <string.h>

#include "render.h"
#include "burst.h"


#define BURST_LIFE 100


//spawns a text burst
void SpawnBurst(BURST * b, const VECTOR2 * p, const char * msg)
{
   if(b && p && msg)
   {
      strncpy(b->text, msg, BURST_TEXT_SIZE);
      b->text_len = (int)strlen(b->text);

      b->pos.x = p->x;
      b->pos.y = p->y;

      b->life = BURST_LIFE;
   }
}

//updates the burst
void UpdateBurst(BURST * b)
{
   if(b)
      b->life--;
}

//draws the burst
void DrawBurst(BURST * b)
{
   float fade;

   if(b)
   {
      fade = (float)b->life / (float)BURST_LIFE;
      RenderText(b->pos.x - 0.2619f * (float)b->text_len * fade, b->pos.y - 0.297625f * fade - 1.0f + 1.0f/fade, 0.0f,
         0.5f * fade, 0.0f, fade, fade, b->text);
   }
}
