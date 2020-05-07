// Generates the three forms of Koch curve: spatial, Newtonian spacetime, and Minkowski spacetime.
#include "stdafx.h"
#include "bmp.h"
#include <fstream>
#include <complex>
typedef complex<double> Complex;

static double width = 700; 
static double height = 1524;

inline void saveSVG(const string &fileName, vector<Vector2d> &points)
{
  static ofstream svg;
  svg.open(fileName.c_str());
  svg << "<svg width = \"" << (int)height << "\" height = \"" << (int)width << "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;

  for (int i = 0; i < (int)points.size(); i++)
  {
    if (i==0)
      svg << "<path d = \"M " << points[i][1] << " " << width - points[i][0];
    else
      svg << " L " << points[i][1] << " " << width - points[i][0];
  }
  svg << "\" fill=\"none\"";
  svg << " stroke-width = \"3\" stroke=\"black\" />\n";
  svg << "</svg>" << endl;
  svg.close();
}

// Spatial Koch
inline void recurseKoch(vector<Vector2d> &points, Vector2d &point, double flip)
{
  double l = (point - points.back()).norm();
  if (l < 4.0)
  {
    points.push_back(point);
    return;
  }

  Vector2d dir = point - points.back();
  Vector2d mid = points.back() + 0.5*dir;
  Vector2d side(dir[1], -dir[0]);
  side *= flip;
  Vector2d next = mid + side*0.25;
  recurseKoch(points, next, -flip);
  recurseKoch(points, point, -flip);
}

// Newtonian version of Koch curve
inline void recurseWeierstrass(vector<Vector2d> &points, Vector2d &point, double flip)
{
  double l = (point - points.back())[1];
  if (l < 4.0)
  {
    points.push_back(point);
    return;
  }

  Vector2d dir = point - points.back();
  Vector2d mid = points.back() + 0.5*dir;
  Vector2d side(l,0);
  side *= flip;
  Vector2d next = mid + side*0.25;
  recurseWeierstrass(points, next, -flip);
  recurseWeierstrass(points, point, -flip);
}

// Minkowskian version of the Koch curve
inline void recurseMinkowski(vector<Vector2d> &points, Vector2d &point, double flip)
{
  double l = (point - points.back())[1];
  if (l < 4.0)
  {
    points.push_back(point);
    return;
  }
  Vector2d ideal(0.25*flip, 0.5);
  Vector2d dir = point - points.back();
  Vector2d right = Vector2d(1, 1) / sqrt(2.0);
  Vector2d left = Vector2d(-1, 1) / sqrt(2.0);
  double k2 = dir.dot(right) / dir.dot(left);
  double scale = sqrt(dir.squaredNorm() / (0.5*k2 + 0.5/k2));
  Vector2d res = right * ideal.dot(right)*sqrt(k2) + left * ideal.dot(left) / sqrt(k2);

  Vector2d next = points.back() + scale*res;
  recurseMinkowski(points, next, -flip);
  recurseMinkowski(points, point, -flip);
}

int chapter7Figure12()
{
  {
    vector<Vector2d> points;
    points.push_back(Vector2d(width / 4.0, 0.0));
    Vector2d point(width / 4.0, height);
    recurseKoch(points, point, 1.0);
    saveSVG("koch_triplet1.svg", points);
  }
  {
    vector<Vector2d> points;
    points.push_back(Vector2d(width / 4.0, 0.0));
    Vector2d point(width / 4.0, height);
    recurseWeierstrass(points, point, 1.0);
    saveSVG("koch_triplet2.svg", points);
  }
  {
    vector<Vector2d> points;
    points.push_back(Vector2d(width / 4.0, 0.0));
    Vector2d point(width / 4.0, height);
    recurseMinkowski(points, point, 1.0);
    saveSVG("koch_triplet3.svg", points);
  }
  return 0;
}
