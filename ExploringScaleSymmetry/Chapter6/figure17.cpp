// Generates an image of the colour-symmetric and scale-symmetric three-colour automaton, on a symmetric hexagon of seed points.
// The method generates to a larger image first (called out), then downscales to a smaller one. The result of this is that
// the dense and repeated mixture of all three colours in the Koch-like fractal regions appears as as a constant colour (a mix of all three). 
#include "stdafx.h"
#include "bmp.h"
#include "spectrumToRGB.h"
#include <set>

// resolution:
static int automataSize = 81 * 3 * 3 * 3;
// the colours of the states (colours[0] is just background). 
static Vector3d colours[5] = { Vector3d(0.5, 0.5, 0.5), Vector3d(40, 220, 70) / 255.0, Vector3d(250, 210, 0) / 255.0, Vector3d(255, 0, 0) / 255.0, Vector3d(20, 0, 255) / 255.0 };

static int width = (automataSize * 31) / 10; 
static int height = width;
static int fullWidth = width;

void putpixel(vector<BYTE> &image, const Vector2i &pos, const Vector3d &col)
{
  if (pos[0] < 0 || pos[0] >= width || pos[1] < 0 || pos[1] >= height)
    return;
  int ind = 3 * (pos[0] + width*pos[1]);
  for (int i = 0; i < 3; i++)
    image[ind + i] = (BYTE)(255.0*col[i]);
}

void putpix(vector<BYTE> &out, const Vector2i &pos, int shade)
{
  if (pos[0] < 0 || pos[0] >= fullWidth || pos[1] < 0 || pos[1] >= fullWidth)
    return;
  out[pos[0] + fullWidth*pos[1]] = shade;
}
int getpix(vector<BYTE> &out, const Vector2i &pos)
{
  if (pos[0] < 0 || pos[0] >= fullWidth || pos[1] < 0 || pos[1] >= fullWidth)
    return 0;
  return out[pos[0] + fullWidth*pos[1]];
}

Vector2i toVector2i(const Vector2d &pos)
{
  Vector2i vec;
  vec[0] = (int)pos[0];
  if ((double)vec[0] > pos[0])
    vec[0]--;
  vec[1] = (int)pos[1];
  if ((double)vec[1] > pos[1])
    vec[1]--;
  return vec;
}

// adds a mid pixel between three parent pixels
bool addMid(vector<BYTE> &out, const Vector2i &pos0, const Vector2i &pos1, const Vector2i &pos2)
{
  Vector2i mid = (pos0 + pos1 + pos2) / 3;
  if (mid[0] < 0 || mid[0] >= width || mid[1] < 0 || mid[1] >= width)
    return false;
  if (getpix(out, mid) > 0)
    return false;
  int col0 = getpix(out, pos0);
  int col1 = getpix(out, pos1);
  int col2 = getpix(out, pos2);
  if (col0 == 0)
    col0 = 1;
  if (col1 == 0)
    col1 = 1;
  if (col2 == 0)
    col2 = 1;
  if (col0 == 0 || col1 == 0 || col2 == 0)
    return false;
  int colour;
  if (col0 == col1 && col0 == col2) // positivity
    colour = col0;
  // ambiguous case
  else if (col0 == col1)
    colour = col0;
  else if (col0 == col2)
    colour = col0;
  else if (col1 == col2)
    colour = col1;
  // if there's not 3 the same, or 2 the same, then there must be 1 of each
  else // 012=3, 013=4, 023=5 123=6 -->  3, 2, 1, 0, i.e. 6 - sum. .: 10-sum
    colour = 10 - (col0 + col1 + col2);
  putpix(out, mid, colour);
  return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
  long s2;
  vector<BYTE> out(width*height); // .bmp pixel buffer
  int startWidth = width;
  memset(&out[0], 0, out.size() * sizeof(BYTE)); 
  Vector2d xaxis[2] = { Vector2d(1, 0), Vector2d(cos(30.0*pi / 180.0), sin(30.0*pi / 180.0)) };
  Vector2d yaxis[2] = { Vector2d(cos(60.0*pi / 180.0), sin(60.0*pi / 180.0)), Vector2d(cos(30.0*pi / 180.0), -sin(30.0*pi / 180.0)) };
  srand(120);
  bool found = true;
  int scale = automataSize;
  Vector2i offset(width / 2, width / 2);

  // initialise
  int count = 1;
  for (int i = -count; i <= count; i++)
  {
    for (int j = -count; j <= count; j++)
    {
      int col = 0;
      if (i == 0 && abs(j) == 1)
        col = 2;
      else if (abs(i) == 1 && j == 0)
        col = 3;
      else if (i == 1 && j == -1)
        col = 4;
      else if (i == -1 && j == 1)
        col = 4;
      else
        col = 1;
      putpix(out, offset + Vector2i(i, j)*scale, col);
    }
  }
  int k = 0;
  do
  {
    found = false;
    int count = (int)(1.0*(double)width / (double)scale);
    for (int i = -count; i < count; i++)
    {
      for (int j = -count; j < count; j++)
      {
        Vector2i pos0 = Vector2i(i, j)*scale;
        Vector2i pos1 = Vector2i(i+1, j)*scale;
        Vector2i pos2 = Vector2i(i, j+1)*scale;
        Vector2i pos3 = Vector2i(i+1, j+1)*scale;
        found |= addMid(out, offset + pos0, offset + pos1, offset + pos2);
        found |= addMid(out, offset + pos1, offset + pos2, offset + pos3);
      }
    }
    if (!found)
      break;
    scale /= 3;
    count = (int)(1.0*(double)width / scale);
    for (int i = -count; i < count; i++)
    {
      for (int j = -count; j < count; j++)
      {
        Vector2i xAxis = Vector2i(2, -1)*scale;
        Vector2i yAxis = Vector2i(1, 1)*scale;
        Vector2i pos0 = i*xAxis + j*yAxis;
        Vector2i pos1 = (i+1)*xAxis + j*yAxis;
        Vector2i pos2 = i*xAxis + (j+1)*yAxis;
        Vector2i pos3 = (i+1)*xAxis + (j+1)*yAxis;
        found |= addMid(out, offset + pos0, offset + pos1, offset + pos2);
        found |= addMid(out, offset + pos1, offset + pos2, offset + pos3);
      }
    }
  } while (found); 

  width /= 2;
  height /= 2;
  vector<BYTE> image(width*height*3); // .bmp pixel buffer
  memset(&image[0], 0, image.size() * sizeof(BYTE));
   
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      int x = 2 * i + (height / 2 - j);
      int y = 2 * j;
      Vector3d col(0, 0, 0);
      col += colours[getpix(out, Vector2i(x, y))];
      col += colours[getpix(out, Vector2i(x, y + 1))];
      col += colours[getpix(out, Vector2i(x + 1, y))];
      col += colours[getpix(out, Vector2i(x + 1, y + 1))];
      putpixel(image, Vector2i(i, j), col / 4.0);
    }
  }

  BYTE* c = ConvertRGBToBMPBuffer(&image[0], width, height, &s2);
  LPCTSTR file = L"three_colour_automaton.bmp";
  SaveBMP(c, width, height, s2, file);
  delete[] c;
}

