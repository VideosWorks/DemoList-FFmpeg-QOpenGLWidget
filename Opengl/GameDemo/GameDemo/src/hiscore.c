/*\
 * hiscore.c - code to handle reading from/writing to hiscore table
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

#ifdef _WIN32
#include <windows.h>
#include "resource.h"
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#if(CFG_GTK)
#include <glib.h>
#include <gtk/gtk.h>
#endif

#include "main.h"
#include "hiscore.h"
#include "render.h"


#ifdef _WIN32
#define HS_FILE "hiscores"
#else
#define HS_FILE ".asteroid_hiscores"
#endif


void Crypt(char *, unsigned int); //encrypts a buffer
char CheckSum(const char *, unsigned int); //gets the checksum of buffer (minus last byte)
#ifdef HAVE_DIALOGS
#  ifdef _WIN32
INT_PTR CALLBACK HiScoreDialog(HWND, UINT, WPARAM, LPARAM); //dialog box callback
#  endif

char * g_name = NULL; //used by HiScoreDialog--destination name buffer
#else
extern char g_szPlayerName[HS_NAME_LENGTH]; //from main.c, the name as it was entered at the prompt
#endif


//reads the table from the hiscore file
void ReadHiScoreTable(HISCORE_TABLE * h)
{
   FILE * f = NULL;
   char * contents = NULL;
   unsigned int len;
   unsigned char entries;

   if(h)
   {
      do
      {
         memset(h, 0, sizeof(HISCORE_TABLE)); //zero the table

         f = OpenHiScoreTable("rb"); //open the file
         if(!f)
            break; //if it can't open, break

         fseek(f, 0, SEEK_END);
         len = ftell(f); //get length of file
         if(len <= 2 || len > (2 + HS_ENTRIES*sizeof(HST_ENTRY)) || ((len - 2) % sizeof(HST_ENTRY)))
            break; //if length is invalid, break

         contents = ast_alloc(len); //alloc space for the file
         fseek(f, 0, SEEK_SET);
         fread(contents, 1, len, f); //read file in

         Crypt(contents, len); //decrypt file

         entries = contents[0]; //get how many entries
         if(entries > HS_ENTRIES || len < 2 + entries*sizeof(HST_ENTRY))
            break; //if invalid entries, break (note that len can be greater than number of entries allows for)

         if(contents[1 + entries*sizeof(HST_ENTRY)] != CheckSum(contents, 2 + entries*sizeof(HST_ENTRY)))
            break; //if checksum fails, break

         memcpy(h->entries, contents + 1, entries*sizeof(HST_ENTRY)); //copy the table into memory

      } while(0);

      if(f)
         fclose(f);
      if(contents)
         free(contents);
   }
}

//returns 1 if the score belongs in the table, 0 if it didn't make it
int IsScoreHi(const HISCORE_TABLE * h, unsigned int s)
{
   int i;
   for(i = 0; i < HS_ENTRIES; i++)
   {
      if(s > h->entries[i].score)
         return 1;
   }
   return 0;
}

#if(CFG_GTK)
struct DIALOG_DATA
{
   char * buf;
   int ok;
};

static gboolean DisplayDialog(gpointer user_data)
{
   struct DIALOG_DATA * data = (struct DIALOG_DATA *)user_data;

   GtkWidget * dialog = gtk_dialog_new_with_buttons("Asteroid High Score",
      NULL, GTK_DIALOG_MODAL,
      GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
      GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
      NULL);
   gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);

   GtkWidget * content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
   GtkWidget * entry = gtk_entry_new();
   gtk_entry_set_max_length(GTK_ENTRY(entry), HS_NAME_LENGTH - 1);
   gtk_entry_set_activates_default(GTK_ENTRY(entry), TRUE);

   gtk_container_add(GTK_CONTAINER(content),
      gtk_label_new("You got a new high score!\nEnter your name (15 chars max):"));
   gtk_container_add(GTK_CONTAINER(content), entry);
   gtk_widget_show_all(dialog);

   int response = gtk_dialog_run(GTK_DIALOG(dialog));
   data->ok = (response == GTK_RESPONSE_ACCEPT);
   if(data->ok)
   {
      strncpy(data->buf, gtk_entry_get_text(GTK_ENTRY(entry)), HS_NAME_LENGTH);
      data->buf[HS_NAME_LENGTH - 1] = '\0';
   }

   gtk_widget_destroy(GTK_WIDGET(dialog));
   gtk_main_quit();
   return FALSE;
}

static int GtkHiScoreDialog(char * buf)
{
   struct DIALOG_DATA data = {buf};
   g_idle_add(DisplayDialog, &data);
   gtk_main();
   return data.ok;
}
#endif

//retrieves the player's name for a hi score
int GetHiScoreName(char * buf)
{
#ifdef HAVE_DIALOGS
#  ifdef _WIN32
   g_name = buf;
   if(DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_HISCORE), NULL, HiScoreDialog))
#  else
   if(GtkHiScoreDialog(buf))
#  endif
   {
      ValidateHiScoreName(buf);
      return 1;
   }
   return 0;
#else
   strcpy(buf, g_szPlayerName); //g_szPlayerName is already validated
   return 1;
#endif
}

//removes unprintable characters from the string
void ValidateHiScoreName(char * name)
{
   char * t;
   char * u;

   for(t = name; *t; ++t) //look at each character in string
   {
      if(!isprint(*t)) //if it's not printable
      {
         for(u = t; *u; ++u) //copy everything down one
            *u = *(u + 1);
      }
   }
}

//adds a score to the hiscore table and writes it to the file
void AddHiScore(HISCORE_TABLE * h, const char * n, unsigned int s, unsigned int l)
{
   FILE * f = NULL;
   char * buf = NULL;
   unsigned int len;
   unsigned char entries;
   int i;

   if(h && n && s && l)
   {
      for(i = 0; i < HS_ENTRIES; i++)
      {
         if(s > h->entries[i].score) //when we find the first entry to put the new score before
         {
            //move everything else down
            memmove(&h->entries[i + 1], &h->entries[i], sizeof(HST_ENTRY) * (HS_ENTRIES - 1 - i));

            //add the entry here
            h->entries[i].score = s;
            strncpy(h->entries[i].name, n, HS_NAME_LENGTH);
            h->entries[i].level = l;
            break;
         }
      }

      do
      {
         for(i = 0; i < HS_ENTRIES; i++)
         {
            if(!(h->entries[i].score)) //when we find a 0 score
               break;
         } //i is now number of entries
         entries = i;

         if(!entries) //if no entries (will always be at least one), break
            break;

         len = 2 + entries*sizeof(HST_ENTRY); //calculate output file length
         buf = ast_alloc(len); //alloc space for it
         buf[0] = entries;     //set up file-byte 0 is number of entries

         memcpy(buf + 1, h->entries, entries*sizeof(HST_ENTRY)); //set up file-remaining bytes (except last byte) are table entries
         buf[len - 1] = CheckSum(buf, len); //set up file-last byte is checksum

         Crypt(buf, len); //encrypt file

         f = OpenHiScoreTable("wb"); //write it
         if(f)
            fwrite(buf, 1, len, f);

      } while(0);

      if(buf)
         free(buf);
      if(f)
         fclose(f);
   }
}

//draws the table
void DrawHiScoreTable(const HISCORE_TABLE * h)
{
   int i;

   if(h)
   {
      glDisable(GL_LIGHTING);
      glColor4f(0.5f, 0.25f, 1.0f, 0.25f);
      glBegin(GL_QUADS);
      glVertex3f(9.0f,  11.5f, 40.0f);
      glVertex3f(31.0f, 11.5f, 40.0f);
      glVertex3f(31.0f, 24.1f, 40.0f);
      glVertex3f(9.0f,  24.1f, 40.0f);
      glEnd();
      glEnable(GL_LIGHTING);

      RenderText(9.31448f, 23.0f, 50.0f, 0.6f, 0.25f, 1.0f, 0.25f, "Player          | Lvl |      Score");
      RenderText(9.31448f, 22.0f, 50.0f, 0.6f, 0.25f, 1.0f, 0.25f, "--------------- | --- | ----------");
      for(i = 0; i < HS_ENTRIES; i++)
      {
         RenderText(9.31448f, 21.0f - (float)i, 50.0f, 0.6f, 0.25f, 1.0f, 0.25f, h->entries[i].name);
         RenderText(19.37144f, 21.0f - (float)i, 50.0f, 0.6f, 0.25f, 1.0f, 0.25f, "| %3lu | %10lu", h->entries[i].level, h->entries[i].score);
      }
   }
}

//returns stdio.h FILE object for hiscore table (close with fclose)
FILE * OpenHiScoreTable(char * mode)
{
#ifdef _WIN32
   return fopen(HS_FILE, mode);
#else
   char szFilename[FILENAME_MAX];
   snprintf(szFilename, FILENAME_MAX - 1, "%s/" HS_FILE, getenv("HOME"));
   szFilename[FILENAME_MAX - 1] = '\0';
   return fopen(szFilename, mode);
#endif
}

//uses simple xor encryption (with a long key) to encrypt a buffer
void Crypt(char * buf, unsigned int len)
{
   // Shitty xor key from random.org.  At least you'll probably have to look at
   // the source to fake a hiscore table?
   // Note: if game play ever gets tweaked, this key should be regenerated so
   // old scores aren't applicable.
   static const unsigned char key[]  = {
      0x8b,0x75,0x2b,0xb1,0xfd,0x1f,0x97,0x7e,0x16,0xab,0xe9,0xea,0xad,0x71,0x43,0x69,
      0x62,0x52,0x41,0xc6,0x6c,0xf1,0xd3,0x60,0x23,0x02,0x52,0x19,0xf3,0xce,0x81,0x29,
      0x69,0xac,0x18,0x85,0xac,0xd2,0x16,0x6a,0x2c,0xd6,0xc9,0x87,0x47,0xc1,0x91,0xbe,
      0x4c,0x83,0x7b,0xee,0x50,0x59,0x40,0xa1,0xb2,0x89,0x82,0x8c,0x85,0xca,0xb4,0x35,
      0x26,0x30,0x64,0x71,0x2b,0x60,0xb1,0x94,0xd4,0xc9,0xd1,0x35,0x7a,0x6f,0xfa,0x12,
      0x66,0xfe,0xb5,0xcb,0x44,0xef,0x54,0x02,0x76,0x25,0xaa,0x47,0xf2,0x0e,0xb9,0x38,
      0x6a,0x34,0x2e,0x51,0x60,0x0c,0xc9,0x81,0x8a,0x59,0x32,0x75,0x71,0xce,0x43,0x5e,
      0xb9,0x2a,0x67,0x90,0x14,0xbc,0xe7,0x9f,0xa4,0xd8,0x5b,0xc0,0xcb,0xb1,0x49,0xbb,
      0x89,0xa1,0x78,0x82,0x4c,0x09,0x83,0xac,0x06,0x6b,0x8e,0xbd,0x16,0x7b,0x85,0x43,
      0xf0,0x8e,0x0c,0x88,0x0c,0x5d,0xfb,0x8b,0xf5,0xdc,0x6d,0xfe,0x85,0xd6,0x67,0x5b,
      0x28,0x7f,0xdb,0x24,0x6e,0xd3,0x1b,0xa4,0x4d,0xa2,0xa1,0xed,0xfb,0xc9,0xad,0x1c,
      0xc9,0x6b,0x50,0x7e,0x76,0x32,0xad,0x79,0x16,0x55,0x5b,0x40,0x7a,0x0b,0xdd,0x40,
      0x96,0xf8,0xb4,0xf1,0x1c,0xcd,0xea,0x75,0xca,0xc0,0xd7,0xd6,0xb4,0xa7,0x9d,0x7a,
      0xb0,0x78,0x5c,0x20,0xdf,0xd6,0x86,0x2c,0x6f,0xf8,0x1f,0x9b,0x52,0x86,0xd3,0x36,
      0x68,0xbf,0xb5,0x4d,0xfa,0xa9,0x3d,0x33,0x1d,0x52,0x4c,0x50,0x30,0x07,0x75,0x14,
      0x70,0x24,0x04,0x7e,0xec,0x81,0x0e,0xf3,0xa1,0xd9,0x5f,0x63,0xf8,0xc4,0xa6,0x22,
   };
   static const int  keylen = sizeof(key)/sizeof(key[0]);

   unsigned int i;

   if(buf)
   {
      for(i = 0; i < len; i++)
         buf[i] ^= ~(char)((int)key[i % keylen] + 0xa3);
   }
}

//gets the checksum of buffer (minus last byte)
char CheckSum(const char * buf, unsigned int len)
{
   unsigned long sum = 0;
   unsigned int i;
   for(i = 0; i < len - 1; i++)
      sum += buf[i];

   return (char)sum;
}

#ifdef _WIN32
//dialog proc for hiscore dialog
INT_PTR CALLBACK HiScoreDialog(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch(uMsg)
   {
      case WM_INITDIALOG:
         return TRUE;

      case WM_COMMAND:
         switch(LOWORD(wParam))
         {
            case IDOK:
               GetDlgItemText(hWndDlg, IDC_NAME, g_name, HS_NAME_LENGTH);
               //fall through
            case IDCANCEL:
               EndDialog(hWndDlg, (LOWORD(wParam) == IDOK));
               return TRUE;
         }
         break;
   }

   return FALSE;
}
#endif
