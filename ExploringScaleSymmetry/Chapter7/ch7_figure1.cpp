// Generates an image representing colliding segments through time. These conserve momentum (if they are the same density) but not energy.
#include "stdafx.h"
#include "bmp.h"
#include <sstream>
#include <fstream>
static int width = 1024;
static int height = 512;

inline void setPixel(vector<BYTE> &out, int x, int y, const Vector3i &col)
{
  if (x < 0 || x >= width || y < 0 || y >= height)
    return;
  int ind = 3 * (x + width * (height - 1 - y));
  out[ind + 0] = col[0];
  out[ind + 1] = col[1];
  out[ind + 2] = col[2];
}

inline void drawPolygon(vector<BYTE> &out, double scale, double h, Vector3i &col)
{
  for (int y = (int)(h*(double)height); y < (int)((h + scale)*(double)height); y++)
  {
    double Y = 1.0 - abs((double)y - (h + 0.5*scale)*(double)height) / (0.5*scale*(double)height);
    Y = max(3.0 / 6.0, (1.0 / 6.0) + 4.0*Y / 6.0);
    double startX = (double)width * scale * Y;
    for (int x = (int)startX; x < (int)(startX + (double)width*scale*1.0 / 6.0); x++)
      setPixel(out, x, y, col);
  }
}

int chapter7Figure1()
{
  vector<BYTE> out(width*height * 3); // .bmp pixel buffer
  memset(&out[0], 255, out.size() * sizeof(BYTE)); 
  long s2;
  int i = 0;
  for (double scale = 1.0; scale > 0.001; scale /= 2.0)
  {
    Vector3i col = i == 0 ? Vector3i(20, 60, 20) : Vector3i(20, 20, 60);
    i = 1 - i;
    for (double y = -0.5; y < 1.0; y+=scale)
      drawPolygon(out, scale, y, col);
  }

  BYTE* c = ConvertRGBToBMPBuffer(&out[0], width, height, &s2);
  LPCTSTR file = L"colliding_segments.bmp";
  SaveBMP(c, width, height, s2, file);
  delete[] c;
  return 0;
}
