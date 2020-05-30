#include "View3D.h"
#include "Screen.h"
#include <string>
#include <sstream>

static int g_type = 2;
static int numEvolvers = 7; 
bool g_fullView3D = false;
using namespace std;

View3D::View3D(int width, int height)
{
  this->width = width;
  this->height = height;

  bigEvolver = new Evolver3D(g_type, true);

  for (int i = 0; i<numEvolvers; i++)
    evolvers[i] = new Evolver3D(g_type, false);

  evolvers[1]->load("data/figure19a.es3", 3);
  evolvers[2]->load("data/figure19c.es4", 4);
  evolvers[3]->load("data/figure19d.es6", 6);
  evolvers[4]->load("data/tree.es8", 8);
  evolvers[4]->setToLetter('g');
  evolvers[5]->load("data/figure21a.es9", 9);
  evolvers[5]->setToLetter('g');
  evolvers[6]->load("data/figure21b.es8", 8);
  evolvers[6]->setToLetter('g');
  for (int i = 1; i <= 6; i++)
    evolvers[i]->randomise();


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

  if (g_fullView3D)
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
  if (g_fullView3D)
    return;
  if (g_type != evolvers[m]->type)
    cout << "Now using type " << evolvers[m]->type << endl;;

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
  if (g_fullView3D)
    return;
  if (g_type != type)
    cout << "Now using type " << type << endl;;

  g_type = evolvers[0]->type = type;

  for (int i = 0; i<numEvolvers; i++)
  {
    evolvers[i]->randomiseMasks(*evolvers[0], 50.0f); // to swap data with probability 50% is the same as setting it, it doesn't matter what it was before
    evolvers[i]->randomise();
  }
}

void View3D::load()
{
  if (g_fullView3D)
    return;
  cout << "Type name of %s file to load, without extension: .es" << g_type << endl;
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

  stringstream strm;
  strm << "data/" << string(key) << ".es" << g_type;
  evolvers[0]->load(strm.str().c_str(), g_type);
  evolvers[0]->randomise();
  for (int i = 1; i<numEvolvers; i++)
  {
    evolvers[i]->set(*evolvers[0]);
    evolvers[i]->randomiseMasks(*evolvers[0], (float)i*2);
    evolvers[i]->randomise();
  }
  cout << "File loaded" << endl;
}

void View3D::setToLetter(char letter)
{
  if (g_fullView3D)
    return;
  for (int i = 0; i<numEvolvers; i++)
  {
    evolvers[i]->setToLetter(letter);
    evolvers[i]->randomise();
  }
}

void View3D::save()
{
  cout << "Type name of %s file to save, without extension: .es" << g_type << endl;
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
 
  stringstream strm;
  strm << "data/" << string(key) << ".es" << g_type;
  FILE* fp;
  if ((fp = fopen(strm.str().c_str(), "wb"))==NULL)
  {
    printf("Cannot open file for writing: %s\n", key);
    return;
  }
  evolvers[0]->write(fp);
  fclose(fp);
  cout << "File saved" << endl;
}

void View3D::update()
{
  if (g_fullView3D)
    bigEvolver->update();
  else
    for (int i = 0; i<numEvolvers; i++)
      evolvers[i]->update();

  if (_kbhit())
  {
    char c = _getch();
    if (c == 'f')
    {
      g_fullView3D = !g_fullView3D;
      int width = 768;
      if (g_fullView3D) // copy data across
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
    if (g_fullView3D) // most features not implemented when in full view
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