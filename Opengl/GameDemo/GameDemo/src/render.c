/*\
 * render.c - generic rendering routines
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//#include <GL/glaux.h> //gone for good, even in windows

#include <stdio.h>
#include <stdarg.h>

#include "main.h"
#include "render.h"
#include "libbmpread/bmpread.h"


#define NOLOADTEX_MSG


//creates and returns a model (display list) that can be later rendered
MODEL CreateModel(const VECTOR3 * vertices, const unsigned int * indices, unsigned int triangles,
                  const VECTOR3 * normals, int vertex_normals, const VECTOR3 * colors, const VECTOR2 * tex_coords,
                  const TEXTURE * texture)
{
   unsigned int i;

   MODEL ret = glGenLists(1);
   glNewList(ret, GL_COMPILE);

   if(texture)
      glBindTexture(GL_TEXTURE_2D, *texture);

   glBegin(GL_TRIANGLES);

   if(!colors)
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

   for(i = 0; i < triangles * 3; i++)
   {
      if(vertex_normals)
         glNormal3fv(normals[indices[i]].v);
      else if(!(i % 3))
         glNormal3fv(normals[i / 3].v);

      if(colors)
         glColor3fv(colors[indices[i]].v);
      if(tex_coords)
         glTexCoord2fv(tex_coords[i].v);

      glVertex3fv(vertices[indices[i]].v);
   }
   glEnd();

   if(texture)
      glBindTexture(GL_TEXTURE_2D, 0);

   glEndList();

   return ret;
}

//draws the model with the current transformation
void DrawModel(MODEL m)
{
   glCallList(m);
}

//creates a texture object from a bitmap file
TEXTURE CreateTexture(const char * name)
{
   TEXTURE ret = 0;
   bmpread_t img;

   if(bmpread(name, 0, &img))
   {
      glGenTextures(1, &ret);
      glBindTexture(GL_TEXTURE_2D, ret);
	  glTexImage2D(GL_TEXTURE_2D, 0, 3, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      bmpread_free(&img);
   }

   return ret;
}

//sets the texture it uses to draw--use 0 to reset it
/*void SetTexture(TEXTURE t)
{
   glBindTexture(GL_TEXTURE_2D, t);
}*/

//outputs text to the screen, at the given position in world coordinates
//uses printf specifiers
//each character is 1.0476 game units wide by 1.1905 game units high
//returns how many characters are in string
int RenderText(float x, float y, float z, float scale, float r, float g, float b, const char * str, ...)
{
   int ret;
   char buf[1024];
   char * c;
   va_list vList;

   va_start(vList, str);
   ret = vsprintf(buf, str, vList);
   va_end(vList);

   glDisable(GL_LIGHTING);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glTranslatef(x, y, z);
   glScalef(0.01f * scale, 0.01f * scale, 0.01f * scale);
   glColor3f(r, g, b);

   for(c = buf; *c; c++)
      glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *c);

   glPopMatrix();
   glEnable(GL_LIGHTING);

   return ret;
}

//wraps vectors around the screen
void WrapVector(VECTOR2 * v)
{
   if(v)
   {
      if(v->x < 0.0f)
         v->x += 40.0f;
      else if(v->x >= 40.0f)
         v->x -= 40.0f;

      if(v->y < 0.0f)
         v->y += 30.0f;
      else if(v->y >= 30.0f)
         v->y -= 30.0f;
   }
}
