/*\
 * asteroid.h - the asteroid struct and related code
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __ASTEROID_H__
#define __ASTEROID_H__


//an asteroid object in the game
typedef struct _ASTEROID
{
   VECTOR2      pos;         //position
   VECTOR2      vel;         //velocity

   float        rot;         //current rotation
   float        rot_vel;     //rotation velocity
   VECTOR3      rot_axis;    //axis of rotation

   float        scale;       //scale (small, medium, or big)

   unsigned int region;      //which region it's in (for efficient clipping)
   CIRCLE       clip_circle; //clipping circle

} ASTEROID;


void FillAsteroidModel(void); //fills the asteroid model

void SpawnAsteroid(ASTEROID *, const VECTOR2 *, float); //resets the asteroid object
void UpdateAsteroid(ASTEROID *);                        //updates an asteroid
void DrawAsteroid(ASTEROID *);                          //draws an asteroid


#endif
