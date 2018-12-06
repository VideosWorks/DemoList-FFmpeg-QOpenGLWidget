/*\
 * burst.h - struct for bursts of text in-game
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __BURST_H__
#define __BURST_H__


#define BURST_TEXT_SIZE 128 //how many characters buffer holds

//a burst of text on the screen
typedef struct _BURST
{
   char         text[BURST_TEXT_SIZE];
   unsigned int text_len;
   VECTOR2      pos;

   unsigned int life;

} BURST;


void SpawnBurst(BURST *, const VECTOR2 *, const char *); //spawns a text burst
void UpdateBurst(BURST *);                               //updates the burst
void DrawBurst(BURST *);                                 //draws the burst


#endif
