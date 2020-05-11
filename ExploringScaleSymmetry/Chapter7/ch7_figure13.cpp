// Generates the Minkowski spacetime version of a Levy C curve of various dimension values, from half down to -1. 
#include "stdafx.h"
#include <iostream>
#include <vector>
#include "/Code/Eigen/Eigen"
#include "/Code/Eigen/StdVector"

using namespace std;
using namespace Eigen;

#include <fstream>
#include <windows.h>
#include <stdio.h>       // for memset

static ofstream svg;
static double scale = 1600.0;

static void openSVG(const string &fileName)
{
  svg.open(fileName.c_str());
  svg << "<svg width = \"" << (int)scale << "\" height = \"" << (int)(scale*1.05) << "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;
}

static int icount = 0;
static void saveSVG(const Vector2d &offset, const vector<Vector3d> &points)
{
  double s = 2.0;
  svg << "<path d = \"M " << scale*(s*points[0][2] + offset[1]) << " " << scale*(1.0 - s*points[0][1] - offset[0]);
  for (int i = 1; i < (int)points.size(); i++)
    svg << " L " << scale*(s*points[i][2] + offset[1]) << " " << scale*(1.0 - s*points[i][1] - offset[0]);
  string cols[] = { "blue", "red", "grey", "orange", "orange", "orange" };
  svg << "\" fill=\"none\" stroke-width=\"2\" stroke=\"" << cols[icount++] << "\" />\n";
}

static void closeSVG()
{
  svg << "</svg>" << endl;
  svg.close();
}

static vector<Vector3d> curve;
static double roothalf = 1.0 / sqrt(2.0);
static Vector3d u(roothalf, -roothalf, 0);
static Vector3d v(roothalf, roothalf, 0);

static void addKochChild(int order, const Vector3d &p0, const Vector3d &p1, double k)
{
  Vector3d dir = p1 - p0;
  double dt = dir[0];
  Vector3d dirn = dir;
  dirn[0] = 0;
  double dist = dirn.norm();

  Vector3d orth(0.0, dir[2], -dir[1]); // spatially orthogonal vector
  if (dist == 0)
    orth = Vector3d(0, -1, 0);
  else
    orth.normalize();
  double s = dist / dt;
  double lift = (dt / 2.0)*sqrt((1.0 - k*k)*(1.0-s*s));
  Vector3d mid = p0 + dirn*0.5 + orth*lift + Vector3d(dt/2.0, 0, 0);

  if (order > 0)
    addKochChild(order - 1, p0, mid, k);
  curve.push_back(mid);
  if (order > 0)
    addKochChild(order - 1, mid, p1, k);
}

int chapter7Figure13()
{
  openSVG("Minkowski_Levy_C_Curve.svg");
  double dims[] = { 0.75, 0.5, 0, -1 };
  for (int d = 0; d < 4; d++)
  {
    double dimension = dims[d]; 
    // here we convert from the dimension value to the child line bend angle
    double time0 = pow(2.0, 1.0 - dimension);
    double a = 0.5;
    double b = -time0;
    double c = 0.5;
    double b24ac = b*b - 4.0*a*c;
    double angle = (-b + sqrt(b24ac)) / (2.0*a);

    // from this angle we convert to a 'downScale', which is how much the proper time scales down with each child
    Vector3d time(1, 0, 0);
    time = u*u.dot(time) / angle + v*v.dot(time)*angle;
    double downScale = 1.0 / time[0];

    curve.push_back(Vector3d(0, 0, 0));
    Vector3d curveEnd(1, 0, 0);
    addKochChild(8, curve[0], curveEnd, downScale);
    curve.push_back(curveEnd);
    saveSVG(Vector2d(0.95, 0.4), curve);
  }
  closeSVG();
  return 0;
}