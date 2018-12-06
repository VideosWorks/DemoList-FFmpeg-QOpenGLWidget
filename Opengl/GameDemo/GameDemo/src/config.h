/*\
 * config.h - build-time configuration
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __CONFIG_H__
#define __CONFIG_H__


#define CFG_VERSION "@Asteroid_BUILD_VERSION@"
#define CFG_RES_INSTALL_PATH "@Asteroid_BUILD_RES_INSTALL_PATH@"
#define CFG_SOUND 0
#define CFG_GTK 0
//"@Asteroid_BUILD_SOUND@"
//"@Asteroid_BUILD_GTK@"
// #if(!CFG_SOUND)
// #  define NO_SOUND
// #endif

#if(defined(_WIN32))
#  define HAVE_DIALOGS
#endif


#endif
