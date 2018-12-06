/*\
 * intersect.h - defines models for clipping
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __INTERSECT_H__
#define __INTERSECT_H__


typedef VECTOR2 POINT2;

typedef struct _LINE
{
   POINT2 a, b;

} LINE;

typedef struct _CIRCLE
{
   POINT2 center;
   float radius;

} CIRCLE;

typedef struct _POLY
{
   POINT2 p[3];

} POLY;


int PointInCircle(const POINT2 *, const CIRCLE *); //whether the point is in the circle
int PointInPoly(const POINT2 *, const POLY *);     //whether the point is in the poly
int PolyInCircle(const POLY *, const CIRCLE *);    //whether the poly intersects the circle
int PolyInPoly(const POLY *, const POLY *);        //whether the poly intersects the poly

void SetPoly(POLY *, float, float, float, float, float, float, const VECTOR2 *, float); //sets a polygon from the model's position, plus a rotation


#endif
