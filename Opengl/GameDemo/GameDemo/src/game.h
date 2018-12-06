/*\
 * game.h - declarations needed for the game "engine"
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __GAME_H__
#define __GAME_H__


#define DEG_TO_RAD 0.0174532925f //pi / 180


#define MAX_PLAYER_SHOTS       4 //max player shots


void TogglePause(void);//toggles between paused and unpaused

void InitGame(void);   //called once to set up game
void NewGame(void);    //called to start the game over
void NextLevel(void);  //clears the screen, goes to next level
void UpdateGame(void); //updates the world
void Render(void);     //renders one frame

unsigned int GetRegion(const VECTOR2 *, float); //gets a region code from a position

void NewShot(const VECTOR2 *, const VECTOR2 *, float, int); //adds a shot to the list of shots
void NewAlien(int);             //adds an alien to the list of aliens
void NewPowerup(void);          //adds a powerup
void NewBurst(const VECTOR2 *, const char *, ...); //spawns a text burst
void DestroyAllAsteroids(int);  //destroys all active asteroids

#define KEY_MASK_ACCEL 0x00000001 //masks for the different keys
#define KEY_MASK_SHOOT 0x00000002
#define KEY_MASK_LEFT  0x00000004
#define KEY_MASK_RIGHT 0x00000008
void ProcKey(int, int); //handles key-presses


#endif
