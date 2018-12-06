/*\
 * intersect.c - code that determines collision detections
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#include <math.h>

#include "render.h"
#include "game.h"
#include "intersect.h"


int LinesIntersect(const LINE *, const LINE *);
int LineInCircle_NoEndPtCheck(const LINE *, const CIRCLE *);


//determines whether a point is within a circle
int PointInCircle(const POINT2 * p, const CIRCLE * c)
{
   float dx, dy;

   if(!p || !c)
      return 0;

   dx = p->x - c->center.x;
   dy = p->y - c->center.y;

   return ((dx*dx + dy*dy) < (c->radius * c->radius));
}

//returns true if the point is contained within the polygon
int PointInPoly(const POINT2 * p, const POLY * t)
{
   int i, intersections = 0;
   LINE l1, l2;

   if(!p || !t)
      return 0;

   l1.a.x = -1.0f;
   l1.a.y = -1.0f;
   l1.b.x = p->x;
   l1.b.y = p->y;

   for(i = 0; i < 3; i++)
   {
      l2.a.x = t->p[i].x;
      l2.a.y = t->p[i].y;
      l2.b.x = t->p[(i + 1) % 3].x;
      l2.b.y = t->p[(i + 1) % 3].y;

      if(LinesIntersect(&l1, &l2))
         intersections++;
   }

   return (intersections & 1); //returns 1 if odd, 0 if even # intersections
}

//determines whether a poly intersects a circle
int PolyInCircle(const POLY * t, const CIRCLE * c)
{
//*
   int i;
   LINE l;
//*/

   if(!t || !c)
      return 0;

   if(PointInCircle(&t->p[0], c) || PointInCircle(&t->p[1], c) || PointInCircle(&t->p[2], c))
      return 1; //if any of the points of the polygon are within the circle, it intersects

//*
   for(i = 0; i < 3; i++)
   {
      l.a.x = t->p[i].x;
      l.a.y = t->p[i].y;
      l.b.x = t->p[(i + 1) % 3].x;
      l.b.y = t->p[(i + 1) % 3].y;

      if(LineInCircle_NoEndPtCheck(&l, c)) //check each line of the polygon to see if it's in the circle
         return 1;
   }
//*/

   return 0;
}

//returns true if the polygons passed as parameters intersect
int PolyInPoly(const POLY * t1, const POLY * t2)
{
   int i, j;
   LINE l1, l2;

   if(!t1 || !t2)
      return 0;

   if(PointInPoly(&t1->p[0], t2) || PointInPoly(&t1->p[1], t2) || PointInPoly(&t1->p[2], t2))
      return 1; //if any of the points of t1 are in t2

   for(i = 0; i < 3; i++)
   {
      l1.a.x = t1->p[i].x;
      l1.a.y = t1->p[i].y;
      l1.b.x = t1->p[(i + 1) % 3].x;
      l1.b.y = t1->p[(i + 1) % 3].y;

      for(j = 0; j < 3; j++)
      {
         l2.a.x = t2->p[j].x;
         l2.a.y = t2->p[j].y;
         l2.b.x = t2->p[(j + 1) % 3].x;
         l2.b.y = t2->p[(j + 1) % 3].y;

         if(LinesIntersect(&l1, &l2)) //if the line intersects any line in t2
            return 1;
      }
   }

   return 0; //no intersection
}

//sets a polygon from the model's position, plus a rotation
void SetPoly(POLY * t, float x1, float y1, float x2, float y2, float x3, float y3, const VECTOR2 * pos, float rot)
{
   float sin_theta, cos_theta;

   if(t && pos)
   {
      cos_theta = (float)cos(rot * DEG_TO_RAD);
      sin_theta = (float)sin(rot * DEG_TO_RAD);
      t->p[0].x = x1 * cos_theta - y1 * sin_theta + pos->x;
      t->p[0].y = y1 * cos_theta + x1 * sin_theta + pos->y;
      t->p[1].x = x2 * cos_theta - y2 * sin_theta + pos->x;
      t->p[1].y = y2 * cos_theta + x2 * sin_theta + pos->y;
      t->p[2].x = x3 * cos_theta - y3 * sin_theta + pos->x;
      t->p[2].y = y3 * cos_theta + x3 * sin_theta + pos->y;
   }
}

//returns true if the lines passed as parameters intersect on the segments between their endpoints
int LinesIntersect(const LINE * l1, const LINE * l2)
{
   float dx1, dx2, dy1, dy2, ex, ey, den, num1, num2, t1, t2;

   if(!l1 || !l2)
      return 0;

   dx1 = l1->b.x - l1->a.x; //difference in x coords of line 1
   dx2 = l2->b.x - l2->a.x; //difference in x coords of line 2
   dy1 = l1->b.y - l1->a.y; //difference in y coords of line 1
   dy2 = l2->b.y - l2->a.y; //difference in y coords of line 2
   ex  = l1->a.x - l2->a.x; //difference in x coords between first point in both lines
   ey  = l1->a.y - l2->a.y; //difference in y coords between first point in both lines

   den = dy2*dx1 - dx2*dy1; //denominator of both equations
   num1 = dx2*ey - dy2*ex;  //numerator of eq 1
   num2 = dx1*ey - dy1*ex;  //numerator of eq 2

   if(den == 0.0f) //if denominator is 0
   {
      if(num1 == 0.0f || num2 == 0.0f) //if either numerator is 0, they're coincident
         return 1;

      return 0; //if just denominator is 0, they're parallel
   }

   t1 = num1 / den;
   t2 = num2 / den;

   if(t1 >= 0.0f && t1 <= 1.0f && t2 >= 0.0f && t2 <= 1.0f)
      return 1;

   return 0;
}

int LineInCircle_NoEndPtCheck(const LINE * l, const CIRCLE * c)
{
   float dx, dy, det, d_sq, discr, x_t1, x_t2, y_t1, y_t2, x, y;
   LINE t;

   if(!l || !c)
      return 0;

   if(l->b.x > l->a.x) //set t.a and t.b to be topleft and bottomright endpoints of line, respectively
   {
      t.a.x = l->a.x - c->center.x; //but move them all over by the circle's center (center
      t.b.x = l->b.x - c->center.x; //of circle should be 0,0 for these calculations)
   }
   else
   {
      t.a.x = l->b.x - c->center.x;
      t.b.x = l->a.x - c->center.x;
   }
   if(l->b.y > l->a.y)
   {
      t.a.y = l->a.y - c->center.y;
      t.b.y = l->b.y - c->center.y;
   }
   else
   {
      t.a.y = l->b.y - c->center.y;
      t.b.y = l->a.y - c->center.y;
   }

   dx = t.b.x - t.a.x;                         //distance between x coords
   dy = t.b.y - t.a.y;                         //distance between y coords
   det = t.a.x*t.b.y - t.b.x*t.a.y;            //determinant of matrix
   d_sq = dx*dx + dy*dy;                       //length of line, squared
   discr = d_sq*c->radius*c->radius - det*det; //discriminant - length squared times radius squared minus determinant squared

   if(discr < 0.0f) //if discriminant less than zero, no intersection
      return 0;

   x_t1 =  det * dy; //temp result 1 during x position calculation
   y_t1 = -det * dx; //temp result 1 during y position calculation

#define sign(x) (((x) < 0) ? -1 : 1)

   discr = (float)sqrt(discr); //take square root of discriminant
   x_t2 = sign(dy) * dx * discr; //temp result 2 during x position calculation
   y_t2 = sign(dy) * dy * discr; //temp result 2 during y position calculation

#undef sign

   x = (x_t1 + x_t2) / d_sq; //compute x coord of intersection 1
   y = (y_t1 + y_t2) / d_sq; //compute y coord of intersection 1
   if(x >= t.a.x && x <= t.b.x && y >= t.a.y && y <= t.b.y) //if it's on the line segment, we intersect
      return 1;

   if(x_t2 != 0.0f || y_t2 != 0.0f) //if there's another intersection point
   {
      x = (x_t1 - x_t2) / d_sq; //compute x coord of intersection 2
      y = (y_t1 - y_t2) / d_sq; //compute y coord of intersection 2
      if(x >= t.a.x && x <= t.b.x && y >= t.a.y && y <= t.b.y) //if it's on the line segment, intersect
         return 1;
   }

   return 0; //no intersection
}
