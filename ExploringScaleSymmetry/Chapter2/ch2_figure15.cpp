// Thomas Lowe, 2020.
// Generates the 1.5D 'mushroom fractal', as a vector graphic .svg file
#include <fstream>
#include "stdafx.h"
#include "bmp.h"

// The number of iterations
static int order = 3; 

static void addChild(vector<Vector2d> &ps, int order, const Vector2d &p02, const Vector2d &p12, bool flip = false)
{
  if (order < 0)
    return;
  const Vector2d p0 = p02;
  const Vector2d p1 = p12;
  Vector2d dir(p1[1] - p0[1], p0[0] - p1[0]);
  if (flip)
    dir = -dir;

  Vector2d across = (p1 - p0)*0.25;
  Vector2d p0b = p0 + across;
  Vector2d p0h = p1 - across;
  Vector2d p0c, p0d, p0e, p0f, p0g;
  bool swap;
  p0c = p0b + dir*0.25;
  p0d = p0c + dir*0.25;
  p0e = p0 + (p1 - p0)*0.5 + dir*0.5;
  p0g = p0h + dir*0.25;
  p0f = p0g + dir*0.25;
  swap = !flip;
  addChild(ps, order - 1, p0, p0b, flip);
  ps.push_back(p0b);
  addChild(ps, order - 1, p0b, p0c, swap);
  ps.push_back(p0c);
  addChild(ps, order - 1, p0c, p0d, flip);
  ps.push_back(p0d);
  addChild(ps, order - 1, p0d, p0e, flip);
  ps.push_back(p0e);
  addChild(ps, order - 1, p0e, p0f, flip);
  ps.push_back(p0f);
  addChild(ps, order - 1, p0f, p0g, flip);
  ps.push_back(p0g);
  addChild(ps, order - 1, p0g, p0h, swap);
  ps.push_back(p0h);
  addChild(ps, order - 1, p0h, p1, flip);
}

static ofstream svg;
static double svgwidth = 900.0;
static void saveSVG(const string &fileName, const vector<Vector2d> points)
{
  double scale = 0.85;
  svg.open(fileName.c_str());
  svg << "<svg width = \"" << (int)svgwidth << "\" height = \"" << (int)svgwidth << "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;
  svg << "<path d = \"M " << scale*svgwidth*points[0][0] + svgwidth / 2.0 << " " << scale*svgwidth*points[0][1] + svgwidth / 2.0;
  for (int i = 1; i < (int)points.size(); i++)
  {
    svg << " L " << scale*svgwidth*points[i][0] + svgwidth/2.0 << " " << scale*svgwidth*points[i][1] + svgwidth / 2.0;
  }
  svg << "\" fill=\"transparent\" stroke=\"black\"/>\n";
  svg << "</svg>" << endl;
  svg.close();
}


int chapter2Figure15()
{
  vector<Vector2d> ps;
  ps.push_back(Vector2d(-0.5, -0.5));
  Vector2d p2(0.5, -0.5);
  addChild(ps, order + 2, ps[0], p2, true); // recursive function creates the first edge of the snowflake
  ps.push_back(p2);
  saveSVG("mushroom_curve.svg", ps);
  return 0;
}
