// Calculates the area of the expanded disk tree (or its boundary or centre points), and draws the expanded image.
// Calling this for multiple values of r, can be used to calculate the fractal dimension and unnormalised Minkowski content.
#include "stdafx.h"
#include "bmp.h"
#include <set>
#include <sstream>
#include <fstream>

// define one of the below, otherwise it computes for the full disk tree, to measure its area.
//#define SURFACE // computes for just the boundary of the disk tree, to measure the boundary length.
//#define POINTS // computes just for one point in each disk tree, to count the number of disks.
static double r = 0.025 / 2.0; // this is the expansion radius

static int width = 4000;
static int height = width; 

static void setPixel(vector<BYTE> &out, const Vector2i &pos, double col)
{
  if (pos[0] < 0 || pos[0] >= width || pos[1] < 0 || pos[1] >= height)
    return;
  int ind = 3 * (pos[0] + width * (height - 1 - pos[1]));
  out[ind + 0] = (int)(255.0*col);
  out[ind + 1] = (int)(255.0*col);
  out[ind + 2] = (int)(255.0*col);
}

static double distanceToTree(Vector2d &p)
{
  double scale = 1.0;
  double ratio = 2.0 / 3.0;
  for (int i = 0; i < 8; i++)
  {
    if ((p - Vector2d(0, 0.5)).norm() > 0.5)
      if (i == 0)
        break; // not in set
    if ((p - Vector2d(0, 1.0 / 3.0)).norm() < 1.0 / 3.0)
      break; // is in set
    double ss = p.squaredNorm();
    p /= ss;
    scale /= ss;

    if (p[1] > 1.0)
    {
      p = Vector2d(0, 3.0) - 2.0 * p;
      scale *= 2.0;
    }
    p[0] = fmod(100 + p[0] + 0.5, 1.0) - 0.5;
  }
#if defined SURFACE
  double d = ((p - Vector2d(0, 1.0 / 3.0)).norm() - 1.0/3.0) / scale; 
#elif defined POINTS
  double d = (p - Vector2d(0, 1.0 / 3.0)).norm() / scale; 
#else
  double d = ((p - Vector2d(0, 1.0 / 3.0)).norm() - 1.0/3.0) / scale; 
#endif
  return d;
}

int chapter9Figure12_disktree()
{
  long s2;
  vector<BYTE> out(width*width * 3); // .bmp pixel buffer
  memset(&out[0], 255, out.size() * sizeof(BYTE)); // background is white

  double count = 0.0;
  double xyMin = -0.6;
  double xyMax = 0.6;
  double zMin = -0.1;
  double zMax = 1.1;
  for (int i = 0; i < width; i++)
  {
    double x = xyMin + (xyMax - xyMin)*(((double)i) + 0.5) / (double)width;
    for (int j = 0; j < width; j++)
    {
      double y = zMin + (zMax - zMin)*(((double)j) + 0.5) / (double)width;
      Vector2d p(x, y);
      double d = distanceToTree(p);
#if defined SURFACE
      if (abs(d) < r)
#elif defined POINTS
      if (d < r)
#else 
      if (d < 0.0)
#endif
      {
        count++;
        setPixel(out, Vector2i(i, j), 0);
      }
    }
  }
  double step = (xyMax - xyMin) / (double)width;
  double area = count * step*step;
  cout << "r: " << r << ", area: " << area << endl;

  BYTE* c = ConvertRGBToBMPBuffer(&out[0], width, height, &s2);
  LPCTSTR file = L"disk_tree_points.bmp";
  SaveBMP(c, width, height, s2, file);
  delete[] c;
  return 0;
}
