#include <stdlib.h>
#include <glut.h>
#include <stdio.h>
#include "Core3D.h"
#include "view3D.h"
#include <time.h>

static Core3D core; // Just a singleton
static int width = 768;
static int height = 768;
extern bool g_fullView;

static void init(void)
{    
  srand((unsigned int)time(NULL));
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  core.init(width, height);
}

static void display(void)
{
  core.render();
  glutSwapBuffers();
}

static void update(void)
{
  core.update(1.0f/60.0f); // avoids outliers
  glutPostRedisplay();
}

static void reshape(int w, int h)
{
  width = w;
  height = h;
}

static void mouse(int button, int state, int x, int y) 
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN)
         if (!g_fullView)
         {
           if (x > 256 && x<512 && y>0 && y<256) // clicked the master
           {
             core.view->setMaster(0);
           }
           else
           {
             int c = 1;
             for (int Y = 256; Y<768; Y+=256)
             {
               for (int X = 0; X<768; X+=256)
               {
                 if (x>X && x<(X+256) && y>Y && y<(Y+256))
                   core.view->setMaster(c);
                 c++;
               }
             }
           }
         }
         break;
      default:
         break;
   }
}

int chapter6Figure19()
{
  int n = 1;
  char *name = "automata3D";
  int *argc = &n;
  char **argv = &name;
  
  glutInit(argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(width, height); 
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutIdleFunc(update); // starts the updating
  glutMainLoop();
  return 0;
}