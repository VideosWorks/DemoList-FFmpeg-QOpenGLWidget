/*\
 * explosion.h - the explosion struct and related code
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__


#define NUM_EXPLOSION_PARTICLES 32
typedef struct _EXPLOSION
{
   VECTOR2      pos[NUM_EXPLOSION_PARTICLES];       //position of each particle
   VECTOR2      vel[NUM_EXPLOSION_PARTICLES];       //velocity of each particle
   unsigned int fade_time[NUM_EXPLOSION_PARTICLES]; //each one fades at a different rate

   unsigned int life;

} EXPLOSION;


void SpawnExplosion(EXPLOSION *, const VECTOR2 *, int); //spawns a new explosion
void UpdateExplosion(EXPLOSION *);                      //updates it
void DrawExplosion(EXPLOSION *);                        //draws it


#endif
