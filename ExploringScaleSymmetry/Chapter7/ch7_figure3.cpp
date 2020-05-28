// Thomas Lowe, 2020.
// Generates image of scale-symmetric billiards. The balls are all the same density, and collide elastically 
#include "stdafx.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "imagewrite.h"
#include <set>

// These are the primary free parameters
static double speed = 200.0;
static double radius = 60.0;

static const int width = 1024;
static const int height = 1024;

static void putpixel(vector<BYTE> &out, const Vector2i &pos, int shade)
{
  if (pos[0] < 0 || pos[0] >= width || pos[1] < 0 || pos[1] >= width)
    return;
  int ind = 3 * (pos[0] + width*pos[1]);
  out[ind + 0] = out[ind + 1] = out[ind + 2] = shade;
}
static int getpixel(vector<BYTE> &out, const Vector2i &pos)
{
  if (pos[0] < 0 || pos[0] >= width || pos[1] < 0 || pos[1] >= width)
    return 0;
  int ind = 3 * (pos[0] + width*pos[1]);
  return out[ind + 0];
}

static double ang = atan(4.0);
static double l = sin(ang);
static double w = cos(ang);

namespace
{
  struct Ball
  {
    Vector2d pos0;
    Vector2d fwd;
    bool flip, big;
    double scale;
    double time0;
  };
}

// Recursive function to add child balls to the structure
static void addBalls(vector<Ball, aligned_allocator<Ball> > &balls, Ball &ball)
{
  balls.push_back(ball);
  if (ball.scale < 1.0/32.0)
    return;
  // we need to add new balls that will collide with this in the time period 0 to 1
  Vector2d side(-ball.fwd[1], ball.fwd[0]);
  if (ball.flip)
    side = -side;
  double a = 2.0*(pi / 2.0 - ang);
  Ball ball0;
  ball0.pos0 = ball.pos0 + ball.fwd*speed*ball.scale;
  ball0.scale = ball.scale / 2.0;
  ball0.flip = !ball.flip;

  ball0.time0 = ball.time0 + 1.0*ball.scale;
  double b = (pi + a) / 2.0;
  ball0.fwd = -ball.fwd*cos(b) - side*sin(b);
  ball0.big = true;
 
  Ball ball1;
  ball1.pos0 = ball0.pos0 + 2.0*(speed + radius)*ball.scale*(ball.fwd*cos(a) + side*sin(a));
  ball1.scale = ball.scale / 2.0;
  ball1.flip = ball.flip;
  ball1.time0 = ball.time0 + 1.0*ball.scale;
  double c = b + a;
  ball1.fwd = -ball.fwd*cos(c) - side*sin(c);
  ball1.big = false;

  ball0.pos0 += ball0.fwd * (ball.scale + ball0.scale)*radius;
  ball1.pos0 += ball1.fwd * (ball.scale + ball1.scale)*radius;

  addBalls(balls, ball0);
  addBalls(balls, ball1);
}

static void drawDisk(const Vector2d &pos, vector<BYTE> &out, double rad, int shade)
{
  for (int x = (int)(pos[0] - rad); x <= (int)(pos[0] + rad); x++)
    for (int y = (int)(pos[1] - rad); y <= (int)(pos[1] + rad); y++)
      if (sqr(x - pos[0]) + sqr(y - pos[1]) <= sqr(rad))
        putpixel(out, Vector2i(x, y), 255 - shade);
}

int chapter7Figure3()
{
  
  vector<BYTE> out(width*height * 3); // .bmp pixel buffer
  memset(&out[0], 255, out.size() * sizeof(BYTE)); // background is grey

  vector<Ball, aligned_allocator<Ball> > balls;
  Ball ball;
  ball.pos0 = Vector2d(412.0, -50.0);
  ball.fwd = Vector2d(0, 1);
  ball.scale = 1.0;
  ball.time0 = 0;
  ball.flip = false;
  ball.big = false;

  addBalls(balls, ball);

  // now draw the balls
  for (auto &ball : balls)
  {
    for (int i = 0; i < 2; i++)
    {
      double t0 = i == 0 ? 0.6 : 0.0;
      double t1 = i == 0 ? 0.9 : 2.0;  
      double step = i == 0 ? 0.02 : 0.05;
      for (double time = t0; time < t1; time += step)
      {
        double shade = 127.0*(time - t0) / (t1 - t0);
        double t = (time - ball.time0) / ball.scale;
        Vector2d side(-ball.fwd[1], ball.fwd[0]);
        if (ball.flip)
          side = -side;

        Vector2d p0 = ball.pos0;
        Vector2d p1 = ball.pos0 + ball.fwd * ball.scale * speed;
        double a = 2.0*(pi / 2.0 - ang);
        Vector2d p2 = p1 + (ball.fwd*cos(a) + side*sin(a)) * ball.scale * speed;

        t = fmod(t + 4000.0, 4.0); // the +4000 just ensures the fmod will work with negative numbers in the given range
        Vector2d p;
        if (t < 1.0)
          p = p0 + (p1 - p0)*t;
        else if (t < 2.0)
          p = p1 + (p2 - p1)*(t - 1.0);
        else if (t < 3.0)
          p = p2 + (p1 - p2)*(t - 2.0);
        else
          p = p1 + (p0 - p1)*(t - 3.0);
        if (i == 0)
          drawDisk(p, out, ball.scale * radius, (int)shade);
        else
          drawDisk(p, out, 1.0, 127);

        p2 += (ball.fwd*cos(a) + side*sin(a)) * ball.scale * 2.0 * radius;
        p1 = p2 + (ball.fwd*cos(a) + side*sin(a)) * ball.scale * speed;
        p0 = p1 + (ball.fwd*cos(2.0*a) + side*sin(2.0*a)) * ball.scale * speed;

        if (t < 1.0)
          p = p0 + (p1 - p0)*t;
        else if (t < 2.0)
          p = p1 + (p2 - p1)*(t - 1.0);
        else if (t < 3.0)
          p = p2 + (p1 - p2)*(t - 2.0);
        else
          p = p1 + (p0 - p1)*(t - 3.0);
        if (i == 0)
          drawDisk(p, out, ball.scale * radius, (int)shade);
        else
          drawDisk(p, out, 1.0, 127);

        double rad = radius;
        if (ball.big)
          rad *= 2.6;
        p = p0 + (ball.fwd*cos(2.0*a) + side*sin(2.0*a)) * ball.scale * (radius + rad);
        drawDisk(p, out, ball.scale * rad, 255);
      }
    }
  }

  stbi_write_png("scale_symmetric_billiards.png", width, height, 3, &out[0], 3 * width);
  return 0;
}

