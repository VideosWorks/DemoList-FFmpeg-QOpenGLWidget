/*\
 * render.h - defines generic model structures et al.
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __RENDER_H__
#define __RENDER_H__


//a 2D vector
typedef struct _VECTOR2
{
   union
   {
      struct
      {
         float x, y; //x and y components
      };

      float v[2];
   };

} VECTOR2;

//a 3D vector
typedef struct _VECTOR3
{
   union
   {
      struct
      {
         float x, y, z;
      };

      float v[3];    //three floats that define the vector's position
   };

} VECTOR3;

//a model - now just an opengl list
typedef unsigned int MODEL;

//a texture--an opengl texture thingy
typedef unsigned int TEXTURE;


MODEL CreateModel(const VECTOR3 *, const unsigned int *, unsigned int,
                  const VECTOR3 *, int, const VECTOR3 *, const VECTOR2 *,
                  const TEXTURE *); //makes a model object
void DrawModel(MODEL); //draws a model object

TEXTURE CreateTexture(const char *); //loads a texture from a file
//void SetTexture(TEXTURE);            //uses the texture to draw models

int RenderText(float, float, float, float, float, float, float, const char *, ...); //outputs text

void WrapVector(VECTOR2 *); //wraps vectors around the screen


#endif
