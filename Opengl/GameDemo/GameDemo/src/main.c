/*\
 * main.c - the main source file
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
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if(CFG_GTK)
#include <glib.h>
#include <gtk/gtk.h>
#endif

#include "main.h"
#include "render.h"
#include "game.h"
#include "hiscore.h"
#include "sound.h"


#define ASTEROIDS_ABOUT "Asteroid v" ASTEROIDS_VERSION "; copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay.\n" \
                        "Asteroid is released under the GNU General Public License and as such comes\n" \
                        "with absolutely no warranty of any kind, whatsoever.  Asteroids is originally\n" \
                        "copyright (C) 1979 Atari, Inc.\n\n" \
                        "website: <https://github.com/chazomaticus/asteroid>\n" \
                        "contact: <chaz@chazomatic.us>\n\n" \
                        "Controls: the left and right arrows turn the ship; x accelerates and z shoots.\n" \
                        "Press p to pause and unpause the action.  Right click somewhere in the game\n" \
                        "window to see some menu options."


void PrimeGL(void);  //sets up the state of opengl to have everything look correct
void GameTimer(int); //timer callback to update game state
void Menu(int);      //called when the user selects a command from the menu
void KeyDown(unsigned char, int, int);
void KeyUp(unsigned char, int, int);
void ExtKeyDown(int, int, int);
void ExtKeyUp(int, int, int);
void MouseMotion(int, int);
void MousePressed(int, int, int, int);
void MouseAction(void);


int g_bFullscreen = 0;
int g_iWindowX;
int g_iWindowY;
int g_iWindowWidth;
int g_iWindowHeight;

int g_bShowingCursor = 1;
int g_iMouseCursor;
int g_iHideCursorTime;

#ifndef HAVE_DIALOGS
char g_szPlayerName[HS_NAME_LENGTH];
#endif


//point of entry
int main(int argc, char * argv[])
{
   srand(time(NULL));

#ifndef HAVE_DIALOGS
   puts(ASTEROIDS_ABOUT);
   puts("");
   puts("");

   if(argc > 1 && !strcmp(argv[1], "--help"))
   {
      puts("Usage: asteroid [yourname] [glut_options]");
      puts("");
      puts("If yourname is specified, asteroid won't prompt you for it later.  glut_options");
      puts("are passed to glut so you can control window-creation parameters et al. (see");
      puts("man glutInit for specifics).  yourname must come before glut_options.");
      return 0;
   }

   if(argc > 1)
   {
      strncpy(g_szPlayerName, argv[1], HS_NAME_LENGTH - 1);
      g_szPlayerName[HS_NAME_LENGTH - 1] = '\0';
   }
   else
   {
      printf("Enter your name as it should appear in the hiscore table (%d chars max):\n", HS_NAME_LENGTH - 1);
      fputs("> ", stdout);
      fgets(g_szPlayerName, HS_NAME_LENGTH, stdin);
      puts("");
   }

   ValidateHiScoreName(g_szPlayerName);

   printf("Your name will appear as '%s' in the hiscore table.\n", g_szPlayerName);
   puts("To change this, you must restart asteroid (unfortunately).");
   puts("");
   puts("");
#endif

   glutInitWindowSize(640, 480);
   glutInit(&argc, argv);
#if(CFG_GTK)
   gtk_init(&argc, &argv);
#endif
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutCreateWindow("Asteroid v" ASTEROIDS_VERSION);

   g_iMouseCursor = glutGet(GLUT_WINDOW_CURSOR);

   glutDisplayFunc(Render);

   glutCreateMenu(Menu);
   glutAddMenuEntry("New Game (F2)", 0);
   glutAddMenuEntry("(Un)Pause (p)", 4);
   glutAddMenuEntry("(Un)Mute (m)", 5);
   glutAddMenuEntry("Toggle Fullscreen (f)", 3);
#ifdef HAVE_DIALOGS
   glutAddMenuEntry("About... (F1)", 2);
#endif
   glutAddMenuEntry("Quit (Esc)", 1);
   glutAttachMenu(GLUT_RIGHT_BUTTON);

   glutIgnoreKeyRepeat(1);
   glutKeyboardFunc(KeyDown);
   glutKeyboardUpFunc(KeyUp);
   glutSpecialFunc(ExtKeyDown);
   glutSpecialUpFunc(ExtKeyUp);
   glutMouseFunc(MousePressed);
   glutMotionFunc(MouseMotion);
   glutPassiveMotionFunc(MouseMotion);

   InitSounds();   //sets up SDL_mixer and loads sounds
   PrimeGL();      //sets up GL
   MouseAction();  //sets up cursor hiding stuff
   InitGame();     //inits game engine

   GameTimer(0); //start game update timer

   glutMainLoop();
   return 0;
}

void * ast_alloc(size_t size)
{
   void * ret = malloc(size);
   if(!ret)
      ast_error("Asteroid has run out of memory.  Exiting...\n");

   return ret;
}

void ast_error(char * msg)
{
#ifdef _WIN32
      MessageBox(NULL, msg, "Asteroid error", MB_OK | MB_ICONERROR);
#else
      fputs(msg, stderr);
#endif
      exit(1);
}

void ast_warn(char * msg)
{
#ifdef _WIN32
      MessageBox(NULL, msg, "Asteroid warning", MB_OK | MB_ICONERROR);
#else
      fputs(msg, stderr);
#endif
}

//correctly sets up the state of gl so everything looks good
void PrimeGL(void)
{
   float light_params[4];

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-20.0f, 20.0f, -15.0f, 15.0f, 1.0f, 1000.0f);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(20.0f, 15.0f, 100.0f, 20.0f, 15.0f, 0.0f, 0.0f, 1.0f, 0.0f);

   glClearDepth(1.0f);
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

   //glPointSize(1.0f);

   glEnable(GL_DEPTH);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   glEnable(GL_COLOR_MATERIAL);

   glShadeModel(GL_SMOOTH);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   light_params[0] = light_params[1] = light_params[2] = 0.15f;
   light_params[3] = 1.0f;
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_params);
   light_params[0] = light_params[1] = 1.0f;
   light_params[2] = light_params[3] = 0.0f;
   glLightfv(GL_LIGHT0, GL_POSITION, light_params);
   glEnable(GL_LIGHT0);

   glEnable(GL_TEXTURE_2D);
}

//glut callback for rotation timer
void GameTimer(int v)
{
   glutTimerFunc(14, GameTimer, v); //set up next timer

   if(g_bShowingCursor && glutGet(GLUT_ELAPSED_TIME) > g_iHideCursorTime)
   {
      g_bShowingCursor = 0;
      glutSetCursor(GLUT_CURSOR_NONE);
   }

   UpdateGame();
}

#if(CFG_GTK)
static gboolean DisplayDialog()
{
   GtkWidget * dialog = gtk_message_dialog_new(NULL,
      GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, ASTEROIDS_ABOUT);
   gtk_window_set_title(GTK_WINDOW(dialog), "About Asteroid v" ASTEROIDS_VERSION);
   gtk_dialog_run(GTK_DIALOG(dialog));

   gtk_widget_destroy(GTK_WIDGET(dialog));
   gtk_main_quit();
   return FALSE;
}

static void GtkAboutBox()
{
   g_idle_add(DisplayDialog, NULL);
   gtk_main();
}
#endif

//called when a menu item is selected
void Menu(int id)
{
   if(id == 0)
      NewGame();
   else if(id == 1)
      exit(0);
#ifdef HAVE_DIALOGS
   else if(id == 2)
   {
#  ifdef _WIN32
      MessageBox(NULL, ASTEROIDS_ABOUT, "About Asteroid v" ASTEROIDS_VERSION, MB_OK | MB_ICONINFORMATION);
#  else
      GtkAboutBox();
#  endif
   }
#endif
   else if(id == 3)
   {
      if(g_bFullscreen)
      {
         glutPositionWindow(g_iWindowX, g_iWindowY);
         glutReshapeWindow(g_iWindowWidth, g_iWindowHeight);
      }
      else
      {
         g_iWindowX      = glutGet(GLUT_WINDOW_X);
         g_iWindowY      = glutGet(GLUT_WINDOW_Y);
         g_iWindowWidth  = glutGet(GLUT_WINDOW_WIDTH);
         g_iWindowHeight = glutGet(GLUT_WINDOW_HEIGHT);

         glutFullScreen();
      }

      g_bFullscreen = !g_bFullscreen;
   }
   else if(id == 4)
      TogglePause();
   else if(id == 5)
      ToggleMute();
}

//called when an ASCII key is pressed
void KeyDown(unsigned char key, int x, int y)
{
   if(key == 'z' || key == 'Z')
      ProcKey(1, KEY_MASK_SHOOT);
   else if(key == 'x' || key == 'X')
      ProcKey(1, KEY_MASK_ACCEL);
   else if(key == 'f' || key == 'F')
      Menu(3);
   else if(key == 'p' || key == 'P')
      Menu(4);
   else if(key == 27) //esc
      Menu(1);
   else if(key == 'm' || key == 'M')
      Menu(5);
/*
   else if(key == '!')
      DestroyAllAsteroids(0);
   else if(key == '@')
      NewPowerup();
   else if(key == '#')
      NewAlien(1);
   else if(key == '$')
      NewAlien(0);
   else if(key == '%')
      NextLevel();
//*/
}

//when an ASCII key is released
void KeyUp(unsigned char key, int x, int y)
{
   if(key == 'z' || key == 'Z')
      ProcKey(0, KEY_MASK_SHOOT);
   else if(key == 'x' || key == 'X')
      ProcKey(0, KEY_MASK_ACCEL);
}

//when an extended key is pressed
void ExtKeyDown(int key, int x, int y)
{
   if(key == GLUT_KEY_LEFT)
      ProcKey(1, KEY_MASK_LEFT);
   else if(key == GLUT_KEY_RIGHT)
      ProcKey(1, KEY_MASK_RIGHT);
   else if(key == GLUT_KEY_F2)
      Menu(0);
#ifdef HAVE_DIALOGS
   else if(key == GLUT_KEY_F1)
      Menu(2);
#endif
}

//when an extended key is released
void ExtKeyUp(int key, int x, int y)
{
   if(key == GLUT_KEY_LEFT)
      ProcKey(0, KEY_MASK_LEFT);
   else if(key == GLUT_KEY_RIGHT)
      ProcKey(0, KEY_MASK_RIGHT);
}

void MouseMotion(int a, int b)
{
   MouseAction();
}

void MousePressed(int a, int b, int c, int d)
{
   MouseAction();
}

void MouseAction(void)
{
   g_bShowingCursor = 1;
   glutSetCursor(g_iMouseCursor);

   g_iHideCursorTime = glutGet(GLUT_ELAPSED_TIME) + 1000;
}
