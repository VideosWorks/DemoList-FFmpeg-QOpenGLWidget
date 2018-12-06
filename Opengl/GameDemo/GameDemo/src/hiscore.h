/*\
 * hiscore.h - algorithms for the hi score table
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#ifndef __HISCORE_H__
#define __HISCORE_H__


#define HS_NAME_LENGTH 16
#define HS_ENTRIES     10

typedef struct _HST_ENTRY
{
   unsigned int score;                //their score
   char         name[HS_NAME_LENGTH]; //player's name
   unsigned int level;                //what level they were on

} HST_ENTRY;

typedef struct _HISCORE_TABLE
{
   HST_ENTRY entries[HS_ENTRIES];

} HISCORE_TABLE;


void ReadHiScoreTable(HISCORE_TABLE *);        //reads the table from the hiscore file
int  IsScoreHi(const HISCORE_TABLE *,
               unsigned int);                  //returns 1 if the score belongs in the table, 0 if it didn't make it
int  GetHiScoreName(char *);                   //retrieves the player's name for a hi score
void ValidateHiScoreName(char *);              //removes unprintable characters from the string
void AddHiScore(HISCORE_TABLE *, const char *,
                unsigned int, unsigned int);   //adds a score to the hiscore table and writes it to the file
void DrawHiScoreTable(const HISCORE_TABLE *);  //draws the table
FILE * OpenHiScoreTable(char *);               //returns stdio.h FILE object for hiscore table (close with fclose)


#endif
