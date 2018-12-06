/*\
 * geometry.h - functions that deal with specific geometry (e.g., the asteroid model geometry)
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__


void InitGeometry(void);      //sets up geometry before any rendering takes place
MODEL MakeAsteroid(TEXTURE);  //creates an asteroid model
MODEL MakePlayerShip(void);   //creates a player ship model
MODEL MakeAlienShip(void);    //creates an alien ship model


#endif
