// Calculates the expanded area of Ford Disks (or their boundary or their centre points), and renders the expanded shape to file.
// Calculating this area for multiple radii can be used to extract the (unnormalised) fractal content, and the fractal dimension.
#include "stdafx.h"
#include "bmp.h"
#include <set>
#include <sstream>
#include <fstream>

// calculates for the filled ford disks. Or set one of the below: 
//#define SURFACE // calculates for the Ford disk boundary (Ford circles)
//#define POINTS  // calculates for the Ford disk centre points.
static double r = 0.025 / 2.0; // this is the expansion radius.

static int width = 3200; 
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

static double distanceToDisks(Vector2d &p)
{
  double scale = 1.0; 
  int numIterations = 8;
  int i = 0;
  for (i = 0; i < numIterations; i++)
  {
    p[0] -= floor(p[0]);
    if (p[0] > 0.5)
      p[0] = 1.0 - p[0];
    if ((p - Vector2d(0, 0.5)).squaredNorm() < 0.5*0.5)
      break; // definitely inside
    if (p[1] < 0.0 || p[1] > 0.5)
      break;
    double p2 = p.squaredNorm();
    // needs a sphere inverse
    scale *= p2;
    p /= p2; // no, has to be further back 
  }
#if defined POINTS
  double d = (p - Vector2d(0, 0.5)).norm() * scale; // the 0.4 is slightly more averaging than 0.5
#else
  double d2 = abs(p[1]) * scale;
  double d3 = abs(p[1] - 1.0) * scale;
  double d = ((p - Vector2d(0, 0.5)).norm() - 0.5) * scale; // the 0.4 is slightly more averaging than 0.5
  d = min(d, d2);
  if (i != 0)
    d = min(d, d3);
#endif
  return d;
}

int chapter9Figure12_forddisks()
{
  long s2;
  vector<BYTE> out(width*width * 3); // .bmp pixel buffer
  memset(&out[0], 255, out.size() * sizeof(BYTE)); // background is white

  double count = 0.0;
  double xyMin = 0.0;
  double xyMax = 1.0;
  double zMin = -0.025;
  double zMax = 1.025;
  for (int i = 0; i < width; i++)
  {
    double x = xyMin + (xyMax - xyMin)*(((double)i) + 0.5) / (double)width;
    for (int k = 0; k < width; k++)
    {
      double z = zMin + (zMax - zMin)*(((double)k) + 0.5) / (double)width;
      Vector2d p(x, z);
      double d = distanceToDisks(p);
#if defined SURFACE
      if (abs(d) < r)
#elif defined POINTS
      if (d < r)
#else 
      if (d < 0.0)
#endif
      {
        count++;
        setPixel(out, Vector2i(i, k), 0);
      }
    }
  }
  double stepX = (xyMax - xyMin) / (double)width;
  double stepY = (zMax - zMin) / (double)width;
  double area = count * stepX*stepY;
  cout << "r: " << r << ", area: " << area << endl;

  BYTE* c = ConvertRGBToBMPBuffer(&out[0], width, height, &s2);
  LPCTSTR file = L"ford_disks_points.bmp";
  SaveBMP(c, width, height, s2, file);
  delete[] c;
  return 0;
}
