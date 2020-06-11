// Thomas Lowe, 2020.
// Generates 8 example co-similar fractals. In these images the white+yellow shape is a larger version of the white shape. The white+pink shape is a larger version of the pink shape (which is also the yellow shape).
#include "stdafx.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "imagewrite.h"
#include <set>
#include <fstream>
#include <iostream>
#include "basics.h"

// when defined it renders 8 examples. Otherwise it generates a new random example every keypress.
#define SHOW_EXAMPLES

static int width = 1600;
static int height = 1200;

static void putpixel(vector<BYTE> &out, const Vector2d &point, double green, double blue)
{
  Vector2i pos;
  pos[0] = int((point[0] + 1.0) * (double)height*0.8);
  pos[1] = int((point[1] + 0.7) * (double)height*0.8);

  if (pos[0] < 0 || pos[0] >= width || pos[1] < 0 || pos[1] >= height)
    return;
  int ind = 3 * (pos[0] + width*pos[1]);
  out[ind + 0] = 255;
  out[ind + 1] = (int)(green * 255.0);
  out[ind + 2] = (int)(blue * 255.0);
}

static double randomVal(double start, double end)
{
  return start + (end - start)*(double)(rand() % 1000) / 1000.0;
}

int chapter3Figure17()
{
  vector<BYTE> out(width*height * 3); // .bmp pixel buffer
  // seven nice indices of randomly chosen transforms
  int indices[] = { 1148124, 1930700, 2129150, 2536776, 2930738, 3372686, 3472683, 0 };
  double maxang = 3.14;
  srand(5);

#if defined SHOW_EXAMPLES
  for (int ii = 0; ii < 8; ii++)
  {
#else
  for (int ii = 0; ii < 4000000; ii++)
  {
    _getch();
#endif
    Vector2d pos[2];
    Vector2d posi[2];
    double angle1;
    double angle2;
    double anglei1;
    double scalei;
    double anglei2;
#if defined SHOW_EXAMPLES
    stringstream strm;
    strm << "data/cosimilar_" << ii << ".txt";
    std::ifstream is;
    is.open(strm.str(), ios::in);
    if (!is.is_open())
    {
      cout << "couldn't find file: " << strm.str() << endl;
      return 1;
    }
    is >> angle1 >> angle2 >> pos[0][0] >> pos[0][1] >> pos[1][0] >> pos[1][1] >> anglei1 >> scalei >> posi[0][0] >> posi[0][1] >> anglei2 >> posi[1][0] >> posi[1][1];
    is.close();
#else
    angle1 = 0;
    pos[0] = Vector2d(0.5, 0);
    angle2 = randomVal(-maxang, maxang);
    pos[1] = Vector2d(randomVal(-1.0, 1.0), randomVal(-1.0, 1.0));
    anglei1;// = randomVal(-maxang, maxang);
    scalei = randomVal(0.6, 0.8);
    posi[0] = Vector2d(randomVal(-0.5, 0.5), randomVal(-0.5, 0.5));
    anglei2 = randomVal(-maxang, maxang);
    posi[1] = Vector2d(randomVal(-0.5, 0.5), randomVal(-0.5, 0.5));
#endif

    cout << "index: " << ii << " transforms- angle 1: " << angle1 << ", pos[0]: " << pos[0].transpose() << ", angle2: " << angle2 << ", pos[1]: " << pos[1].transpose() << ", anglei1: " << anglei1 << ", scalei: " << scalei << ", posi[0]: " << posi[0].transpose() << ", anglei2: " << anglei2 << ", posi[1]: " << posi[1].transpose() << endl;
    Matrix2d mat[2];  // mat + pos i for shape 1 equals shape i in orientation defined by rmat + rpos i (which can have any scale) 
    Matrix2d mati[2];
    Vector2d v1(cos(angle1), sin(angle1));
    Vector2d v2(cos(angle2), sin(angle2));
    mat[0] << v1[0], v1[1],
      -v1[1], v1[0];
    mat[1] << v2[0], v2[1],
      -v2[1], v2[0];

    mati[0] << cos(anglei1), sin(anglei1),
      -sin(anglei1), cos(anglei1);
    mati[0] *= scalei;

    mati[1] << cos(anglei2), sin(anglei2),
      -sin(anglei2), cos(anglei2);
    mati[1] *= scalei;

    memset(&out[0], 0, out.size() * sizeof(BYTE));
    vector<Vector2d> points[2], newPoints[2];
    newPoints[1].push_back(Vector2d(0, 0));
    int top = 28;
    for (int i = 0; i < top; i++) 
    {
      int p = i % 2;
      // place latest points in 0, 1 onto layout p to update shape p
      vector<Vector2d> newps;
      for (auto &point : newPoints[0])
        newps.push_back(mati[p] * point + posi[p]);
      for (auto &point : newPoints[1])
        newps.push_back(mati[p] * (mat[p] * point + pos[p]) + posi[p]);
      
      points[p].insert(points[p].end(), newPoints[p].begin(), newPoints[p].end());
      newPoints[p] = newps;
      if (p == 0)
      {
        for (auto &point : newPoints[p])
        {
          if (i >= top - 2)
            putpixel(out, point, 1.0, 1.0);
        }
      }
      else
      {
        for (auto &point : newPoints[p])
        {
          Vector2d p[2] = { mat[0] * point + pos[0], mat[1] * point + pos[1] };
          if (i >= top - 1)
          {
            putpixel(out, p[0], 1, 0);
            putpixel(out, p[1], 0, 1);
          }
        }
      }
    }

    stringstream str;
    str << "shape" << ii << ".png";
    stbi_write_png(str.str().c_str(), width, height, 3, &out[0], 3 * width);
  }
  return 0;
}

