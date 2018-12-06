/*\
 * alien.h - the alien struct and related code
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __ALIEN_H__
#define __ALIEN_H__


//an alien ship
typedef struct _ALIEN
{
   VECTOR2      pos;           //position
   VECTOR2      vel;           //velocity

   float        scale;         //scale of alien (big or small)

   unsigned int time_to_shoot; //how many frames are left until we can shoot again

   float        anim_rot;      //current rotation

   unsigned int region;        //which region it's in (for efficient clipping)
   POLY         clip_poly/*s[3]*/; //clipping polygon

} ALIEN;


void FillAlienModel(void); //fills the alien model

void SpawnAlien(ALIEN *, int);                           //spawns a new alien
void UpdateAlien(ALIEN *, const PLAYER *, const LIST *); //updates the alien
void DrawAlien(ALIEN *);                                 //draws the alien


#endif
