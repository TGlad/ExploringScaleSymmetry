// Generates a disk tree, which is a form of recursive Ford spheres. 
#include "stdafx.h"
#include "bmp.h"
#include <fstream>

// controls the visual detail level
static int numIterations = 5;

static int width = 1024;
static int height = 1024;

inline void putpixel(vector<BYTE> &out, const Vector2i &pos, int shade)
{
  if (pos[0] < 0 || pos[0] >= width || pos[1] < 0 || pos[1] >= height)
    return;
  int ind = 3 * (pos[0] + width*(height-pos[1]));
  out[ind + 0] = out[ind + 1] = out[ind + 2] = shade;
}

int chapter4Figure20()
{
  long s2;
  vector<BYTE> out(width*height * 3); 
  memset(&out[0], 255, out.size() * sizeof(BYTE)); 
   
  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      Vector2d p;
      p[0] = ((double)(x - width / 2)) / ((double)width);
      p[1] = ((double)y) / (double)height;
      double dScale = 1.0;
      double ratio = 2.0/3.0;
      for (int i = 0; i < numIterations; i++)
      {
        if ((p - Vector2d(0, 1.0/3.0)).norm() < 1.0/3.0)
          break; 
        p /= p.squaredNorm();
        if (p[1] > 1.0)
          p = Vector2d(0, 3.0) - 2.0 * p;
        p[0] = fmod(100 + p[0] + 0.5, 1.0) - 0.5;
      }
      if ((p - Vector2d(0, 0.5)).norm() < 0.5)
        putpixel(out, Vector2i(x, y), 0);
    }
  }

  BYTE* c = ConvertRGBToBMPBuffer(&out[0], width, height, &s2);
  LPCTSTR file = L"disk_tree.bmp";
  SaveBMP(c, width, height, s2, file);
  delete[] c;
  return 0;
}
