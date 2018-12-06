/*\
 * player.h - the player struct and related code
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __PLAYER_H__
#define __PLAYER_H__


#define PLAYER_SPAWN_WAIT 100 //how many frames to delay when a player spawns

#define PL_POW_SHIELD    0x00000001
#define PL_POW_RAPIDFIRE 0x00000002


//a player ship
typedef struct _PLAYER
{
   VECTOR2      pos;           //position
   VECTOR2      vel;           //velocity

   float        rot;           //current rotation

   unsigned int powerups;      //flags of what powerups it has

   unsigned int time_to_shoot; //how many frames are left until we can shoot again
   unsigned int wait_time;     //how many frames to wait before letting player move again

   float        anim_rot;      //animation rotation (the ship spinning)

   unsigned int region;        //which region it's in (for efficient clipping)
   POLY         clip_poly;     //clipping polygon

} PLAYER;


void FillPlayerModel(void); //fills the player model

void ResetPlayer(PLAYER *);                //makes a new player
void UpdatePlayer(PLAYER *, unsigned int); //updates a player
void DrawPlayer(PLAYER *);                 //draws a player


#endif
