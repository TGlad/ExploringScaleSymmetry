#include "View3D.h"
#include "Screen.h"
#include <conio.h>
#include <string.h>

static int g_type = 2;
static int numEvolvers = 7; 
bool g_fullView = false;

View3D::View3D(int width, int height)
{
  this->width = width;
  this->height = height;

  bigEvolver = new Evolver3D(g_type, true);

  for (int i = 0; i<numEvolvers; i++)
    evolvers[i] = new Evolver3D(g_type, false);

  printf("3D Fractal Automata Search Tool\n");
  printf("Click on your preferred of the seven systems to bring it to the top and generate six new mutated versions below\n");
  printf("Press key 'l' to load, 's' to save, 'f' to toggle full view, , with this window in focus.\n");
  printf("Press 'z','x','c','.' or 'o' for letter shapes, as shown in figure 20.\n");
  printf("Press 'r' for random and 'h' for plan view of hills.\n");
  printf("Press 'g' for ground view, as shown in Figure 21.\n\n");
  printf("Number keys are mapping families to search within:\n");
  printf("Numbers 1-6 score each parent distance differently. e.g. nearest parent scores more than second nearest.\n");
  printf("Numbers 7-9 are vertically asymmetric, so good for landscapes, use 'g' for side view.\n");
}

void View3D::recordToScreen(Screen* screen)
{
  glClearColor(0.25f, 0.25f, 0.25f, 1.0f);   
  glClear(GL_COLOR_BUFFER_BIT);

  // Ortho mode ---------------------------------------------------
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) screen->width, 0.0, (GLdouble) screen->height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (g_fullView)
  {
    glRasterPos2i(0, 0);
    bigEvolver->draw();
  }
  else
  {
    glRasterPos2i(256, 512);
    evolvers[0]->draw();

    int c = 1;
    for (int j = 1; j>=0; j--)
    {
      for (int i = 0; i<3; i++)
      {
        if (c >= numEvolvers)
          continue;
        glRasterPos2i(256*i, 256*j);
        evolvers[c++]->draw();
      }
    }
  }
}

void View3D::setMaster(int m)
{
  if (g_fullView)
    return;
  if (g_type != evolvers[m]->type)
    printf("Now using type %d\n", evolvers[m]->type);

  g_type = evolvers[m]->type;
  if (m!=0)
    evolvers[0]->set(*evolvers[m]);
  evolvers[0]->randomise();
  for (int i = 1; i<numEvolvers; i++)
  {
    evolvers[i]->set(*evolvers[0]);
    evolvers[i]->randomiseMasks(*evolvers[0], (float)i*2);
    evolvers[i]->randomise();
  }
}

void View3D::resetFromHead(int type)
{
  if (evolvers[0]->getStaticValue[type] == NULL)
    return;
  if (g_fullView)
    return;
  if (g_type != type)
    printf("Now using type %d\n", type);

  g_type = evolvers[0]->type = type;

  for (int i = 0; i<numEvolvers; i++)
  {
    evolvers[i]->randomiseMasks(*evolvers[0], 50.0f); // to swap data with probability 50% is the same as setting it, it doesn't matter what it was before
    evolvers[i]->randomise();
  }
}

void View3D::load()
{
  if (g_fullView)
    return;
  char ext[5];
  sprintf_s(ext, ".es%d", g_type);
  printf("Type name of %s file to load, without extension: ", ext);
  char key[80];
  int c = 0;
  do
  {
    key[c] = _getch();
    if (key[c] == 13)
    {
      key[c] = 0; // null terminate
      printf("\n");
    }
    else
      printf("%c", key[c]);
  } while (key[c++] != 0);
  strcat_s(key, ext);
  evolvers[0]->load(key, g_type);
  evolvers[0]->randomise();
  for (int i = 1; i<numEvolvers; i++)
  {
    evolvers[i]->set(*evolvers[0]);
    evolvers[i]->randomiseMasks(*evolvers[0], (float)i*2);
    evolvers[i]->randomise();
  }
  printf("File loaded\n");
}

void View3D::setToLetter(char letter)
{
  if (g_fullView)
    return;
  for (int i = 0; i<numEvolvers; i++)
  {
    evolvers[i]->setToLetter(letter);
    evolvers[i]->randomise();
  }
}

void View3D::save()
{
  char ext[5];
  sprintf_s(ext, ".es%d", g_type);
  printf("Type name of %s file to save, without extension: ", ext);
  char key[80];
  int c = 0;
  do
  {
    key[c] = _getch();
    if (key[c] == 13)
    {
      key[c] = 0; // null terminate
      printf("\n");
    }
    else
      printf("%c", key[c]);
  } while (key[c++] != 0);
  FILE* fp;
  strcat_s(key, ext);
  if (fopen_s(&fp, key, "wb"))
  {
    printf("Cannot open file for writing: %s\n", key);
    return;
  }
  evolvers[0]->write(fp);
  fclose(fp);
  printf("File saved\n");
}

void View3D::update()
{
  if (g_fullView)
    bigEvolver->update();
  else
    for (int i = 0; i<numEvolvers; i++)
      evolvers[i]->update();

  if (_kbhit())
  {
    char c = _getch();
    if (c == 'f')
    {
      g_fullView = !g_fullView;
      int width = 768;
      if (g_fullView) // copy data across
      {
        width = 512;
        bigEvolver->set(*evolvers[0], true);
        bigEvolver->frame = 1;
        bigEvolver->update(); 
      }
      else
      {
        for (int i = 0; i<numEvolvers; i++)
          evolvers[i]->frame = 1;
      }
      // resize the screen here?
      glutReshapeWindow(width, width);
    }
    if (g_fullView) // most features not implemented when in full view
      return; 
    if (c >= '1' && c <= '9')
    {
      resetFromHead(c + 1 - '1');
      return;
    }
    switch(c)
    {
    case('l'):
      load();
      break;
    case('s'):
      save();
      break;
    case('z'):
    case('x'):
    case('c'):
    case('.'):
    case('o'):
    case('r'):
    case('g'):
    case('h'):
      setToLetter(c);
      break;
    default:
      break;
    }
  }
}