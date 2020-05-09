// Generates the basic fractal curve types in vector file format (.svg). 
// the well-known Levy C, Koch and Dragon curves
#include <fstream>
#include "stdafx.h"
#include "bmp.h"

// This defines the type of curve
static int type = 0; // 0: Levy C curve, 1: Koch curve, 2: Dragon curve 
// This defines the amount of bend in the curve
static double lift = 0.5*tan(30.0 * pi / 180.0); // lift is the height of the mid point when the width is unit length. This is the correct lift for the Koch curve
// The iteration depth of fractal curve. 
static int order = 10;

static void addChild(vector<Vector2d> &ps, int order, const Vector2d &p02, const Vector2d &p12, bool flip = false)
{
  if (order < 0)
    return;
  const Vector2d p0 = p02;
  const Vector2d p1 = p12;
  Vector2d dir(p1[1] - p0[1], p0[0] - p1[0]);
  if (flip)
    dir = -dir;
  Vector2d mid = (p0 + p1)*0.5 + dir * lift;
  bool f1, f2;
  if (type == 0)
    f1 = f2 = flip;
  else if (type == 1)
    f1 = f2 = !flip;
  else if (type == 2)
  {
    f1 = true; f2 = false;
  }
  addChild(ps, order - 1, p0, mid, f1);
  ps.push_back(mid);
  addChild(ps, order - 1, mid, p1, f2);
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


int chapter2Figure3()
{
  vector<Vector2d> ps;
  ps.push_back(Vector2d(-0.5, 0));
  Vector2d p2(0.5, 0.0);
  addChild(ps, order, ps[0], p2, false); // recursive function creates the first edge of the snowflake
  ps.push_back(p2);
  if (type == 0)
    saveSVG("LevyC_curve.svg", ps);
  else if (type == 1)
    saveSVG("Koch_curve.svg", ps);
  else if (type == 2)
    saveSVG("Dragon_curve.svg", ps);
  return 0;
}
