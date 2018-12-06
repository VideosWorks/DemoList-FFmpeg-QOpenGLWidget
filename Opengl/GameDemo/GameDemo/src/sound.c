/*\
 * sound.c - sound api
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#include "config.h"

//define NO_SOUND to disable sound at compile time
#ifndef NO_SOUND

#  ifdef _MSC_VER                      //if using msvc++
#     pragma comment(lib, "SDL")       //tell it to add SDL and SDL_mixer
#     pragma comment(lib, "SDL_mixer") //this is better than adding it in the project because this way, if you don't have the
#  endif                               //SDL libs, vc++ won't bitch at you for not having them when compiling with NO_SOUND

#  include <SDL/SDL.h>
#  include <SDL/SDL_mixer.h>

#  include <stdlib.h>
#  include <string.h>

#  include "main.h"


   int g_enabled = 1; //so we can disable it at runtime
   int g_mute    = 0; //so user can mute at will


#  ifdef CFG_RES_INSTALL_PATH //possibly defined from config.h, if it was available
#     define SOUND_PRE CFG_RES_INSTALL_PATH "/sfx/" //if it was defined, append sfx to directory name
#  else
#     define SOUND_PRE "sfx/" //if it wasn't defined, hope sfx/ is in current directory
#  endif

   const char * g_filenames[] = //these filenames must match (in order) the defines in sound.h
   {
      SOUND_PRE "newlevel.wav",
      SOUND_PRE "spawn_shot.wav",
      SOUND_PRE "spawn_alien.wav",
      SOUND_PRE "spawn_powerup.wav",
      SOUND_PRE "exp_ship.wav",
      SOUND_PRE "exp_asteroid.wav",
      SOUND_PRE "exp_other.wav",
      SOUND_PRE "get_powerup.wav"
   };

#  define NUM_SOUNDS (sizeof(g_filenames) / sizeof(g_filenames[0]))

   Mix_Chunk * g_sounds[NUM_SOUNDS];

#endif


//mostly Scott Barr's code, hacked up by Charles
//inits the SDL_mixer and loads sounds
void InitSounds(void)
{
#ifndef NO_SOUND

   int i;

   if(SDL_Init(SDL_INIT_AUDIO))
   {
      ast_warn("Failed to init SDL; disabling audio.\n");
      g_enabled = 0;
      return;
   }
   atexit(SDL_Quit);

   if(Mix_OpenAudio(22050, AUDIO_S16SYS, 1, 1024))
   {
      ast_warn("Failed to open the SDL audio mixer; disabling audio.\n");
      g_enabled = 0;
      return;
   }
   atexit(Mix_CloseAudio);

   memset(g_sounds, 0, sizeof(g_sounds));

   for(i = 0; i < NUM_SOUNDS; ++i)
      g_sounds[i] = Mix_LoadWAV(g_filenames[i]);

   for(i = 0; i < NUM_SOUNDS; ++i)
   {
      if(!g_sounds[i])
      {
         ast_warn("Failed to load one or more .wav files from " SOUND_PRE "\nThings might not sound right, but you can still play.\n");
         break;
      }
   }

#endif
}

void PlaySoundIndex(int index)
{
#ifndef NO_SOUND

   if(!g_enabled || g_mute || index < 0 || index >= NUM_SOUNDS)
      return;

   Mix_PlayChannel(index, g_sounds[index], 0);

#endif
}

void ToggleMute(void)
{
#ifndef NO_SOUND

   g_mute = !g_mute;

#endif
}
