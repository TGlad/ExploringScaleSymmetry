// Generates a Mobius multiset with a large iteration count, to approximate the limit set, and remove cloudy results.
#include "stdafx.h"
#include "bmp.h"
#include <sstream>
#include <fstream>
#include <iostream>

static int maxIterations = 32; // lower values have more cloudy results
static int strength = 6; // affects the colour scale


static int width;
static int height;

static void setPixel(vector<BYTE> &out, int x, int y, const Vector3i &col)
{
  if (x < 0 || x >= width || y < 0 || y >= height)
    return;
  int ind = 3 * (x + width * (height - 1 - y));
  out[ind + 0] = col[0];
  out[ind + 1] = col[1];
  out[ind + 2] = col[2];
}

const int specSize = 1000;
static Vector3i spectrum[specSize];
static double startRadius = 0.0;
static double minX = 0.0;
static double maxX = 0.875;
static double minY = -0.5;
static double maxY = -0.25;
static double scale = 2.0;
static Vector2d flips[2];
static Vector2d bends[2];
static Vector2d shifts[2];

static void init()
{
  // define the colour gradient as a heatmap (red up to blue)
  for (int i = 0; i < specSize; i++)
  {
    double f = (log((double)(i + 8)) - log(8.0)) / log((double)(specSize + 7));
    double f2 = 1.0 - (1.0 - f)*(1.0 - f);

    double red = 0.5*(cos(f * pi) + 1.0);
    double green = 0.375*(1.0f - cos(f * 2.0 * pi));
    double blue = 0.5*(cos((1.0 - f) * pi) + 1.0);

    double scale = 255.0 * f2;
    spectrum[i][0] = (int)(red*scale);
    spectrum[i][1] = (int)(green*scale);
    spectrum[i][2] = (int)(blue*scale);
  }

  // this defines the two mobius transforms ([0] and [1]).
  flips[0] = Vector2d(1, 0);
  flips[1] = Vector2d(1, 0);
  bends[0] = Vector2d(1, 1) / sqrt(2.0);
  bends[1] = Vector2d(-1, 1) / sqrt(2.0);
  shifts[0] = Vector2d(0, 0); 
  shifts[1] = Vector2d(0, 0);
}

// applies the Mobius transform
Vector2d distort(const Vector2d &point, int i, double &radius)
{
  Vector2d pos = point - bends[i];
  double mult = bends[i].squaredNorm() / pos.squaredNorm();
  pos *= mult;
  radius *= mult*scale;
  pos += bends[i];
  pos = (shifts[i] + pos - (2.0 * flips[i] * pos.dot(flips[i]))) * scale;
  return pos;
}

void recurse(vector<BYTE> &out, int &count, const Vector2d &point, const Vector2d &offset, int depth, const Vector2d &point0, double radius)
{
  for (int i = 0; i<2; i++)
  {
    double rad = radius;
    Vector2d pos = distort(point, i, rad) + offset;
    double dist2 = pos.squaredNorm();
    if (pos[1]-radius >= 0.9) // this exit condition is specific to this choice of mobius transforms.
      continue;
    if (depth == 0)
      count++;
    else
      recurse(out, count, pos, offset, depth - 1, point0, rad);
  }
}

void generateFullMap(vector<BYTE> &out)
{
  for (int Y = 0; Y<height; Y++)
  {
    double y = minY + (maxY - minY)*(double)Y / (double)(height - 1);
    for (int X = 0; X<width; X++)
    {
      double x = minX + (maxX - minX)*(double)X / (double)(width - 1);
      int count = 0;
      recurse(out, count, Vector2d(x, y), Vector2d(x, y), maxIterations, Vector2d(x, y), startRadius);
      if (count == 0)
        setPixel(out, X, Y, Vector3i(0,0,0));
      else
        setPixel(out, X, Y, spectrum[min((count * strength) / 10, specSize - 1)]);
    }
  }
}

int chapter5Figure22()
{
  init();
  long s2;

  width = 28 * 128;
  height = 7 * 128;
  startRadius = (maxX - minX) / width;
  vector<BYTE> out(width*height * 3); // .bmp pixel buffer
  memset(&out[0], 255, out.size() * sizeof(BYTE)); // background is grey

  generateFullMap(out);

  BYTE* c = ConvertRGBToBMPBuffer(&out[0], width, height, &s2);
  LPCTSTR file = L"mobius_multimap.bmp";
  SaveBMP(c, width, height, s2, file);
  delete[] c;
  return 0;
}
