/*\
 * sound.h - sound api
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __SOUND_H__
#define __SOUND_H__


//indices of different sounds, for PlaySoundIndex (these must match g_filenames in sound.c)
#define SND_NEWLEVEL       0  //level starts
#define SND_SPAWN_SHOT     1  //shot spawns
#define SND_SPAWN_ALIEN    2  //alien spawns
#define SND_SPAWN_POWERUP  3  //powerup spawns
#define SND_EXP_SHIP       4  //player explodes
#define SND_EXP_ASTEROID   5  //asteroid explodes
#define SND_EXP_OTHER      6  //other (powerup, alien) explodes
#define SND_GET_POWERUP    7  //collect a powerup


void InitSounds(void);           //inits sounds
void PlaySoundIndex(int index);  //plays a sound given above
void ToggleMute(void);           //turn on/off sound


#endif
