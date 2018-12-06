/*\
 * main.h - currently, just a function to allocate memory, exiting if it fails
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __MAIN_H__
#define __MAIN_H__

#include "config.h"


#define ASTEROIDS_VERSION CFG_VERSION


void * ast_alloc(size_t); //allocates memory, exiting if it fails
void ast_error(char *);   //dumps error message, calls exit(1)
void ast_warn(char *);    //dumps error message, continues


#endif
