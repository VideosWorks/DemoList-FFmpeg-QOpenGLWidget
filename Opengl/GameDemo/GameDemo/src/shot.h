/*\
 * shot.h - the shot struct and related code
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __SHOT_H__
#define __SHOT_H__


#define SHOT_LIFE 50 //how many frames the shot stays alive


//a bullet
typedef struct _SHOT
{
   VECTOR2      pos;          //position
   VECTOR2      vel;          //velocity

   unsigned int life;         //how many frames left until it disappears

   unsigned int region;       //which region it's in (for efficient clipping)

   int          player_owned; //1 if shot from player, 0 otherwise

} SHOT;


void SpawnShot(SHOT *, const VECTOR2 *, const VECTOR2 *, float, int); //makes a new shot
void UpdateShot(SHOT *);                                              //updates a shot
void DrawShot(SHOT *);                                                //draws the shot


#endif
