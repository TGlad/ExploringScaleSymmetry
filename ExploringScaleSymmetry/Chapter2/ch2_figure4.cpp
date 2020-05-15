// Thomas Lowe, 2020.
// Generates the three alternative permutations of the basic fractal curve, in vector file format (.svg). 
#include <fstream>
#include "stdafx.h"
#include "bmp.h"

// This defines the type of curve
static int type = 1; // 1: new type 1, 2: new type 2, 3: new type 3 
// This defines the amount of bend in the curve
static double lift = 0.5*tan(30.0 * pi / 180.0); // lift is the height of the mid point when the width is unit length. This is the correct lift for the Koch curve
static int order = 14;

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
  if (type == 1)
  {
    f1 = flip;
    f2 = !flip;
  }
  else if (type == 2)
  {
    if (order % 2)
    {
      f1 = !flip;
      f2 = flip;
    }
    else
    {
      f1 = flip;
      f2 = !flip;
    }
  }
  else if (type == 3)
  {
    if (order % 2)
    {
      f1 = 1;
      f2 = 0;
    }
    else
    {
      f1 = 0;
      f2 = 1;
    }
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


int chapter2Figure4()
{
  vector<Vector2d> ps;
  ps.push_back(Vector2d(-0.5, 0));
  Vector2d p2(0.5, 0.0);
  addChild(ps, order + 2, ps[0], p2, true); // recursive function creates the first edge of the snowflake
  ps.push_back(p2);
  stringstream stream;
  stream << "new_curve_type_" << type << ".svg";
  saveSVG(stream.str(), ps);
  return 0;
}
