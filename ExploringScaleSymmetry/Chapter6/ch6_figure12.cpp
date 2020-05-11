// Generates the set of 64 diamond-square automata of two colours.
#include "stdafx.h"
#include "bmp.h"
#include <set>

static vector<Vector2i> pointset, pointset2;
static int gap = 4;
static int vgap = 14;
static int depth = 7;
static int eachWidth = 3 * (1 << depth);
static const int rows = 8;
static const int cols = 8;
static int width = (eachWidth + gap) * rows + 1 - gap;
static int height = (eachWidth + vgap) * cols;
static int X = 0;
static int Y = 0;

static void putpix(vector<BYTE> &out, const Vector2i &pos, int shade)
{
  if (pos[0] < 0 || pos[0] > eachWidth || pos[1] < 0 || pos[1] > eachWidth)
    return;
  int ind = 3 * (X*(eachWidth + gap) + pos[0] + width*(Y*(eachWidth + vgap) + pos[1]));
  out[ind + 0] = out[ind + 1] = out[ind + 2] = shade;
}
static int getpix(vector<BYTE> &out, const Vector2i &pos)
{
  if (pos[0] < 0 || pos[0] > eachWidth || pos[1] < 0 || pos[1] > eachWidth)
    return 0;
  int ind = 3 * (X*(eachWidth + gap) + pos[0] + width*(Y*(eachWidth + vgap) + pos[1]));
  return out[ind + 0];
}
static double level = 2;

void square(vector<BYTE> &out, int x0, int y0, int x2, int y2)
{
  int on[] = { X % 2, (X / 2) % 2, (X / 4) % 2, Y % 2, (Y / 2) % 2, (Y / 4) % 2 };
  int x1 = (x0 + x2) / 2;
  int y1 = (y0 + y2) / 2;
  int count = 0; // will be 0 to 5
  count += getpix(out, Vector2i(x0, y0)) / 255;
  count += getpix(out, Vector2i(x0, y2)) / 255;
  count += getpix(out, Vector2i(x2, y0)) / 255;
  count += getpix(out, Vector2i(x2, y2)) / 255;
  if (count == 2)
  {
    if ((getpix(out, Vector2i(x0, y0)) > 0) == (getpix(out, Vector2i(x2, y2)) > 0)) // extra case
      count = 5;
  }
  putpix(out, Vector2i(x1, y1), on[count] ? 255 : 0);
}

void diamond(vector<BYTE> &out, int x0, int y0, int x2, int y2)
{
  int on[] = { X % 2, (X / 2) % 2, (X / 4) % 2, Y % 2, (Y / 2) % 2, (Y / 4) % 2 };
  int x1 = (x0 + x2) / 2;
  int y1 = (y0 + y2) / 2;
  int count = 0; // will be 0 to 5
  count += getpix(out, Vector2i(x0, y1)) / 255;
  count += getpix(out, Vector2i(x2, y1)) / 255;
  count += getpix(out, Vector2i(x1, y0)) / 255;
  count += getpix(out, Vector2i(x1, y2)) / 255;
  if (count == 2)
  {
    if ((getpix(out, Vector2i(x0, y1)) > 0) == (getpix(out, Vector2i(x2, y1)) > 0)) // extra case
      count = 5;
  }
  putpix(out, Vector2i(x1, y1), on[count] ? 255 : 0);
}

static Vector2i toVector2i(const Vector2d &pos)
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

static bool addMid(vector<BYTE> &out, const Vector2d &pos0, const Vector2d &pos1, const Vector2d &pos2)
{
  Vector2i mid = toVector2i((pos0 + pos1 + pos2) / 3.0);
  if (mid[0] < -eachWidth / 2 || mid[0] >= eachWidth / 2 || mid[1] < -eachWidth / 2 || mid[1] >= eachWidth/2)
    return false;
  if (getpix(out, mid) > 0)
    return false;
  int col0 = getpix(out, toVector2i(pos0));
  int col1 = getpix(out, toVector2i(pos1));
  int col2 = getpix(out, toVector2i(pos2));

  if (col0 == 0 || col1 == 0 || col2 == 0)
  {
    return false;
  }
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

int chapter6Figure12()
{
  long s2;
  vector<BYTE> out(width*height * 3); // .bmp pixel buffer
  int startWidth = eachWidth;
  memset(&out[0], 0, out.size() * sizeof(BYTE)); 
  for (X = 0; X < rows; X++)
  {
    for (Y = 0; Y < cols; Y++)
    {
      int start[4][4] = { { 0, 0, 0, 0 },
      { 0, 1, 1, 0 },
      { 0, 1, 1, 0 },
      { 0, 0, 0, 0 } };
      int w = eachWidth / 3;
      for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
          putpix(out, Vector2i(x, y)*w, start[y][x]*255);
      while (w > 1)
      {
        for (int x = 0; x < eachWidth; x += w)
          for (int y = 0; y < eachWidth; y += w)
            square(out, x, y, x + w, y + w);
        // diamonds are more tricky...
        for (int x = 0; x < eachWidth+1; x += w)
        {
          for (int y = 0; y < eachWidth+1; y += w)
          {
            if (y<eachWidth)
              diamond(out, x - w / 2, y, x + w - w / 2, y + w);
            if (x<eachWidth)
              diamond(out, x, y - w / 2, x + w, y + w - w / 2);
          }
        }
        w = w / 2;
      }
    }
  }

  BYTE* c = ConvertRGBToBMPBuffer(&out[0], width, height, &s2);
  LPCTSTR file = L"diamond_square_automata.bmp";
  SaveBMP(c, width, height, s2, file);
  delete[] c;
  return 0;
}

