/*\
 * list.c - code for the generic linked list struct
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#include <stdlib.h>
#include <memory.h>

#include "main.h"
#include "list.h"


//adds a new node to the list
//adjusts the head pointer (pass THE ADDRESS of your head pointer variable as ppHead)
//allocs space for data_size in n->d, and returns the new data area
void * NewListNode(LIST ** ppHead, size_t data_size)
{
   LIST * n = ast_alloc(sizeof(LIST));
   n->d = ast_alloc(data_size);
   memset(n->d, 0, data_size);
   n->next = n->prev = NULL;

   if(ppHead)
   {
      if(*ppHead)
         (*ppHead)->prev = n;
      n->next = (*ppHead);
      (*ppHead) = n;
   }

   return n->d;
}

//removes the entire list
//updates the head pointer to NULL (pass THE ADDRESS of your head pointer variable as ppHead)
void DeleteEntireList(LIST ** ppHead)
{
   LIST * cur;
   LIST * t;

   if(ppHead)
   {
      for(cur = *ppHead; cur; )
      {
         t = cur;
         cur = cur->next;
         free(t->d);
         free(t);
      }

      (*ppHead) = NULL;
   }
}

//removes r from the list
//returns r->next (as it was before r is freed)
//updates the head pointer if necessary (pass THE ADDRESS of your head pointer variable as ppHead)
LIST * RemoveNode(LIST * r, LIST ** ppHead)
{
   LIST * ret = NULL;

   if(r)
   {
      ret = r->next;

      if(r->next)
         r->next->prev = r->prev;
      if(r->prev)
         r->prev->next = r->next;
      else if(ppHead)
         (*ppHead) = r->next;

      free(r->d);
      free(r);
   }

   return ret;
}
