/*\
 * list.h - stuff for a generic linked list
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __LIST_H__
#define __LIST_H__


typedef struct _LIST
{
   struct _LIST * next;
   struct _LIST * prev;

   void         * d;

} LIST;


void * NewListNode(LIST **, size_t); //adds a new node to the list
void   DeleteEntireList(LIST **);      //removes the entire list
LIST * RemoveNode(LIST *, LIST **);  //removes one node from the list


#endif
