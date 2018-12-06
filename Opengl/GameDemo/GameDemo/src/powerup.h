/*\
 * powerup.h - powerups in the game
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __POWERUP_H__
#define __POWERUP_H__


#define POW_TYPE_1UP       0
#define POW_TYPE_HITALL    1
#define POW_TYPE_SHIELD    2
#define POW_TYPE_RAPIDFIRE 3
#define NUM_POWERUPS       4


//a powerup that the player can collect
typedef struct _POWERUP
{
   VECTOR2      pos;         //position
   VECTOR2      vel;         //velocity

   unsigned int type;        //type of powerup it is

   float        anim_rot;    //rotation for animation
   VECTOR3      rot_axis;    //rotation axis

   unsigned int region;      //which region it's in (for efficient clipping)
   CIRCLE       clip_circle; //clipping circle

} POWERUP;


void SpawnPowerup(POWERUP *);  //spawns a new powerup
void UpdatePowerup(POWERUP *); //updates it
void DrawPowerup(POWERUP *);   //draws it


#endif
