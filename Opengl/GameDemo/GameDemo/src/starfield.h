/*\
 * starfield.h - a struct for holding the starfield parameters
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __STARFIELD_H__
#define __STARFIELD_H__


#define NUM_STARS 1024 //how many stars the field has

//a field of stars, to reiterate the obvious
typedef struct _STARFIELD
{
   VECTOR2 stars[NUM_STARS]; //an array of star positions
   float   alpha[NUM_STARS]; //each one's alpha component

   float   rot;              //how far it's twisted

} STARFIELD;


void InitStarfield(STARFIELD *);   //fills the starfield with data
void UpdateStarfield(STARFIELD *); //updates the burst
void DrawStarfield(STARFIELD *);   //draws the burst


#endif
