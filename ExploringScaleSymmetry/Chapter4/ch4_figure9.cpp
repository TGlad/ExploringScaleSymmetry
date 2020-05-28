// Thomas Lowe, 2020.
// 2D bubble fractal. 
#include "stdafx.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "imagewrite.h"
#include <fstream>

// smaller values give more complex structures.
static double k = 3.0;
// controls detail level
static int numIterations = 7; 

static const int width = 1024;
static const int height = 1024;

static void putpixel(vector<BYTE> &out, const Vector2i &pos, int shade)
{
  if (pos[0] < 0 || pos[0] >= width || pos[1] < 0 || pos[1] >= height)
    return;
  int ind = 3 * (pos[0] + width*(height - 1 - pos[1]));
  out[ind + 0] = out[ind + 1] = out[ind + 2] = shade;
}

int chapter4Figure9()
{
  
  vector<BYTE> out(width*height * 3); 
  memset(&out[0], 255, out.size() * sizeof(BYTE)); 
  Vector2d vs[3] = { Vector2d(0, 1), Vector2d(sqrt(3) / 2.0, -0.5), Vector2d(-sqrt(3) / 2.0, -0.5) };
  
  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      Vector2d p;
      p[0] = ((double)(x - width / 2)) / ((double)width / 4.0);
      p[1] = ((double)(y - height / 2)) / ((double)height / 4.0);
      double dScale = 1.0;
      for (int i = 0; i < numIterations; i++)
      {
        double bendDist = 3.0;
        double l0 = (p - vs[0]).squaredNorm();
        double l1 = (p - vs[1]).squaredNorm();
        double l2 = (p - vs[2]).squaredNorm();
        int vi = 2;
        if (l0 < l1 && l0 < l2)
          vi = 0;
        else if (l1 < l0 && l1 < l2)
          vi = 1;
        p += vs[vi] * (bendDist - 1.0);
        double s = sqr(bendDist) / p.squaredNorm();
        p *= s;
        dScale *= s;
        p -= vs[vi] * (p.dot(vs[vi]) * 2.0 - bendDist);
        p *= k;
        dScale *= k;
      }
      double l0 = (p - vs[0]).squaredNorm();
      double l1 = (p - vs[1]).squaredNorm();
      double l2 = (p - vs[2]).squaredNorm();
      int vi = 2;
      if (l0 < l1 && l0 < l2)
        vi = 0;
      else if (l1 < l0 && l1 < l2)
        vi = 1;
      double t = p.dot(vs[vi]);
      double dist = (p - t*vs[vi]).norm() / dScale;
      if (dist < 0.004 )
        putpixel(out, Vector2i(x, y), 0);
    }
  }

  stbi_write_png("bubble_fractal.png", width, height, 3, &out[0], 3 * width);
  return 0;
}
