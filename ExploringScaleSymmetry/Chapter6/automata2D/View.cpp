// Thomas Lowe, 2020.
// Defined the main interaction with the select-and-mutate scheme
#include "View.h"
#include "Screen.h"
#include <string>
#include <iostream>
#include <sstream>

static int g_type = 1;
int g_fullView = 0;
bool g_timeSymmetric = false;
static int numEvolvers = 7;
using namespace std;

View::View(int width, int height)
{
  this->width = width;
  this->height = height;

  evolvers[0] = new Evolver(10); // so we can draw it bigger
  evolvers[0]->depth = 8;
  evolvers[0]->type = 9;
  for (int i = 1; i<numEvolvers; i++)
    evolvers[i] = new Evolver(8);
  evolvers[1]->load("data/figure4.ev9", 9);
  evolvers[2]->load("data/figure5b.ev9", 9);
  evolvers[3]->load("data/figure7a.ev7", 7);
  evolvers[4]->load("data/figure7g.ev7", 7);
  evolvers[5]->load("data/figure24b.ev1", 1);
  evolvers[6]->load("data/figure26-27.ev3", 3);

  printf("Fractal Automata Search Tool\n");
  printf("Click on your preferred of the seven systems to bring it to the top and generate six new mutated versions below\n");
  printf("Press key 'l' to load, 's' to save, 'f' to toggle full view, with this window in focus.\n");
  printf("Press 't' key to toggle time symmetric mode on/off\n");
  printf("Number keys are mapping families to search within:\n");
  printf("'1' figure 24b: state is number of neighbour cells (incl parents and children)\n");
  printf("'2' function of number of neighbours + self\n");
  printf("'3' figure 26 and 27: for complex type\n");
  printf("'4' function of (number of neighbours, self set)\n");
  printf("'5' state is number of neighbours, but independent per level\n");
  printf("'6' parent and cell's 8 neighbours only\n");
  printf("'7' figure 7: pseudo-octagonal symmetry\n");
  printf("'8' pseudo-octagonal symmetry with larger number of parent cells\n");
  printf("'9' figure 4 and 5: square symmetry\n");
}

void View::recordToScreen(Screen* screen)
{
  glColor4f(0.3f, 0.5f, 0.7f, 1.0f);   
  glClear(GL_COLOR_BUFFER_BIT);

  // Ortho mode ---------------------------------------------------
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) screen->width, 0.0, (GLdouble) screen->height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPixelZoom(1.0f, 1.0f); // so we don't have to rearrange in the setPixel call
  if (g_fullView)
  {
    glRasterPos2i(0, 0);
    evolvers[0]->draw();
  }
  else
  {
    glRasterPos2i(0, 512);
    evolvers[0]->drawMask();
    glRasterPos2i(256, 512);
    evolvers[0]->draw();
    glRasterPos2i(512, 512);
    evolvers[0]->bitmaps[7]->generateTexture();
    evolvers[0]->bitmaps[7]->draw();
    glRasterPos2i(640, 512);
    evolvers[0]->bitmaps[6]->generateTexture();
    evolvers[0]->bitmaps[6]->draw();
    glRasterPos2i(704, 512);
    evolvers[0]->bitmaps[5]->generateTexture();
    evolvers[0]->bitmaps[5]->draw();
    glRasterPos2i(736, 512);
    evolvers[0]->bitmaps[4]->generateTexture();
    evolvers[0]->bitmaps[4]->draw();
    glRasterPos2i(752, 512);
    evolvers[0]->bitmaps[3]->generateTexture();
    evolvers[0]->bitmaps[3]->draw();
    glRasterPos2i(760, 512);
    evolvers[0]->bitmaps[2]->generateTexture();
    evolvers[0]->bitmaps[2]->draw();
    glRasterPos2i(764, 512);
    evolvers[0]->bitmaps[1]->generateTexture();
    evolvers[0]->bitmaps[1]->draw();
    if (g_type == 7 || g_type == 8)
    {
      glRasterPos2i(512, 640);
      evolvers[0]->bitmapDuals[7]->generateTexture();
      evolvers[0]->bitmapDuals[7]->draw();
      glRasterPos2i(640, 640);
      evolvers[0]->bitmapDuals[6]->generateTexture();
      evolvers[0]->bitmapDuals[6]->draw();
      glRasterPos2i(704, 640);
      evolvers[0]->bitmapDuals[5]->generateTexture();
      evolvers[0]->bitmapDuals[5]->draw();
      glRasterPos2i(736, 640);
      evolvers[0]->bitmapDuals[4]->generateTexture();
      evolvers[0]->bitmapDuals[4]->draw();
    }

    int c = 1;
    for (int j = 1; j>=0; j--)
    {
      for (int i = 0; i<3; i++)
      {
        glRasterPos2i(256*i, 256*j);
        evolvers[c++]->draw();
      }
    }
  }
}

void View::setMaster(int m)
{
  if (g_type != evolvers[m]->type)
    cout << "Now using type " << evolvers[m]->type << endl;
  g_type = evolvers[m]->type;
  if (m!=0)
    evolvers[0]->set(*evolvers[m]);
  evolvers[0]->frame = 0;
  evolvers[0]->randomise();
  if (!g_fullView)
  {
    for (int i = 1; i<numEvolvers; i++)
    {
      evolvers[i]->randomiseMasks(*evolvers[0], (float)i*2);
      evolvers[i]->randomise();
    }
  }
}

void View::resetFromHead(char c)
{
  if (g_type != evolvers[0]->type)
    cout << "Now using type " << evolvers[0]->type << endl;

  g_type = evolvers[0]->type;
  bool starts[16];
  bool o[] = { 0, 0, 0, 0,
    0, 1, 1, 0,
    0, 1, 1, 0,
    0, 0, 0, 0 };
  bool l[] = { 0, 0, 0, 0,
    1, 1, 1, 1,
    1, 0, 0, 0,
    0, 0, 0, 0 };
  if (c == 0)
    for (int j = 0; j < 16; j++)
      starts[j] = (rand() % 2)==1;
  else if (c == 'o')
    for (int j = 0; j < 16; j++)
      starts[j] = o[j];
  else if (c == 'l')
    for (int j = 0; j < 16; j++)
      starts[j] = l[j];

  for (int i = 0; i<numEvolvers; i++)
  {
    if (c == 0)
      evolvers[i]->randomiseMasks(*evolvers[0], 50.0f); // to swap data with probability 50% is the same as setting it, it doesn't matter what it was before
    evolvers[i]->randomise(starts);
    evolvers[i]->frame = 0; 
  }
}


void View::load()
{
  cout << "Type name of %s file to load, without extension: .ev" << g_type << endl; 
  char key[80];
  int c = 0;
  do
  {
    key[c] = _getch();
    if (key[c] == enterKey)
    {
      key[c] = 0; // null terminate
      printf("\n");
    }
    else
      printf("%c", key[c]);
  } while (key[c++] != 0);
  
  stringstream strm;
  strm << "data/" << string(key) << ".ev" << g_type;
  if (evolvers[0]->load(strm.str().c_str(), g_type))
  {
    evolvers[0]->randomise();
    for (int i = 1; i<numEvolvers; i++)
    {
      evolvers[i]->randomiseMasks(*evolvers[0], (float)i*2);
      evolvers[i]->randomise();
    }
    cout << "File: " << strm.str() << " loaded" << endl;
  }
}


void View::save()
{
  cout << "Type name of %s file to save, without extension: .ev" << g_type << endl;
  char key[80];
  int c = 0;
  do
  {
    key[c] = _getch();
    if (key[c] == enterKey)
    {
      key[c] = 0; // null terminate
      printf("\n");
    }
    else
      printf("%c", key[c]);
  } while (key[c++] != 0);
  stringstream strm;

  strm << "data/" << string(key) << ".ev" << g_type;
  ofstream fp(strm.str(), ios::binary | ios::out);
  if (!fp.is_open())
  {
    cout << "Cannot open file for writing: " << strm.str() << endl;
    return;
  }
  evolvers[0]->write(fp);
  fp.close();
  cout << "File: " << strm.str() << " saved" << endl;
}

void View::update()
{
  if (g_fullView)
    evolvers[0]->update();
  else
    for (int i = 0; i<numEvolvers; i++)
      evolvers[i]->update();

  if (_kbhit())
  {
    switch(_getch())
    {
    case('k') :
      resetFromHead('l');
      break;
    case('o') :
      resetFromHead('o');
      break;
    case('l') :
      load();
      break;
    case('s'):
      save();
      break;
    case('t'):
      g_timeSymmetric = !g_timeSymmetric;
      if (g_timeSymmetric)
        printf("Time symmetric mode enabled\n");
      else
        printf("Time symmetric mode disabled\n");
      for (int i = 0; i<numEvolvers; i++)
      {
        evolvers[i]->randomise();
        evolvers[i]->frame = 0;
      }
      break;
    case('f'):
      {
        g_fullView = (g_fullView + 1)%3;
        evolvers[0]->depth = 8 + g_fullView;
        int width = 768;
        if (g_fullView==1)
          width = 512;
        else if (g_fullView == 2)
          width = 1024;
        evolvers[0]->frame = 0;
        if (evolvers[0]->type == 6 || evolvers[0]->type == 7 || evolvers[0]->type == 8)
          evolvers[0]->update();
        else
          evolvers[0]->randomise();
        // resize the screen here?
        glutReshapeWindow(width, width);
      }
      break;
    
    case('1'):
      evolvers[0]->type = 1;
      resetFromHead();
      break;
    case('2'):
      evolvers[0]->type = 2;
      resetFromHead();
      break;
    case('3'):
      evolvers[0]->type = 3;
      resetFromHead();
      break;
    case('4'):
      evolvers[0]->type = 4;
      resetFromHead();
      break;
    case('5'):
      evolvers[0]->type = 5;
      resetFromHead();
      break;
    case('6'):
      evolvers[0]->type = 6;
      resetFromHead();
      break;
    case('7'):
      evolvers[0]->type = 7;
      resetFromHead();
      break;
    case('8') :
      evolvers[0]->type = 8;
      resetFromHead();
      break;
    case('9') :
      evolvers[0]->type = 9;
      resetFromHead();
      break;
    case('0') :
      evolvers[0]->type = 10;
      resetFromHead();
      break;
    case('-') :
      evolvers[0]->type = 11;
      resetFromHead();
      break;
    default:
      break;
    }
  }
}