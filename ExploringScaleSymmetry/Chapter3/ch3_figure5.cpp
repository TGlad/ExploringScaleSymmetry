// Generates a scale-symmetric tree with a fractal border. Adjust p,c for different tree shapes.
#include <vector>
#include <fstream>
#include "stdafx.h"

// the following are parameters that can be set.
static double p = 2.0;  // p is the pythagoras scale, 2 gives right angles branching. 
static double c = 0.52; // c is the Cantor number, 1/3 gives even (Y-shaped) branching.


struct Section
{
  Vector2d pos, peak;
  Vector2d xAxis, yAxis;
  double width, width2, length;
  double dir;
  vector<Section> children;
  void split();
  void draw(ofstream &svg, const Vector2d &origin, const Vector2d &xAx, const Vector2d &yAx);
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

static double gradient = 0.0; 
static double scale = 1000.0;
static Vector2d offset(1.0, 0);
static double xVal = 0;
static double weight = 0;

inline void Section::draw(ofstream &svg, const Vector2d &origin, const Vector2d &xAx, const Vector2d &yAx)
{
  Vector2d start = origin + xAx*pos[0] + yAx*pos[1];
  Vector2d x = xAx*xAxis[0] + yAx*xAxis[1];
  Vector2d y = xAx*yAxis[0] + yAx*yAxis[1];

  Vector2d corners[] = { start - x*width*0.5, start + x*width*0.5, start + x*0.5*width2 + y*length, start + x*peak[0] + y*(peak[1] + length), start - x*0.5*width2 + y*length };
  double gscale = 0.9*scale;
  svg << "<path d = \"M " << gscale*(corners[4][0] + offset[0]) << " " << scale-gscale*(corners[4][1] + offset[1]);
  for (int i = 0; i < 5; i++)
    svg << " L " << gscale*(corners[i][0] + offset[0]) << " " << scale-gscale*(corners[i][1] + offset[1]);
  svg << "\" fill=\"black\" stroke=\"black\" stroke-width=\"1\" />\n"; \

  Vector2d p = start + y*length;
  for (auto &c : children)
    c.draw(svg, p, x, y);
}

inline void saveSVG(const string &fileName, Section &tree)
{
  xVal = 0;
  weight = 0;
  static ofstream svg;
  svg.open(fileName.c_str());
  svg << "<svg width = \"" << (int)(1.5*scale) << "\" height = \"" << (int)scale << "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;

  tree.draw(svg, Vector2d(0,0), Vector2d(1,0), Vector2d(0,1));
  svg << "</svg>" << endl;
  svg.close();
}

// recursively split each branch
inline void Section::split()
{
  double minLength = 0.005; // this controls the minimum detail level
  if (length <= minLength)
    return;
  Section child1, child2;

  double w2 = pow(width, p);
  double w2b = pow(width2, p);
  double areaLoss = (w2 - w2b) * c;

  double a = pow(areaLoss, 1.0/p);
  double b = pow((w2 + w2b - areaLoss) / 2.0, 1.0 / p);
  double d = pow((w2 + w2b + areaLoss) / 2.0, 1.0 / p);
  double theta = acos((sqr(b) + sqr(d) - sqr(a)) / (2.0*b*d));
  Vector2d apex(d*0.5 - b*cos(theta), b*sin(theta));
  Vector2d B(-d/2.0, 0);
  Vector2d A(d/2.0, 0);
  Vector2d fromB = apex - B;
  Vector2d fromA = apex - A;
  length = (length - a / 2.0) / 2.0;

  child1.pos = B + 0.5*fromB;
  child1.yAxis = Vector2d(-fromB[1], fromB[0]).normalized();
  child1.xAxis = fromB.normalized();
  child1.pos[0] *= dir; 
  child1.yAxis[0] *= dir; 
  child1.xAxis[1] *= dir;
  child1.length = a / gradient;
  child1.width = a;
  child1.width2 = 0;
  child1.dir = -dir; // remove negation for curly
  child1.peak = Vector2d(0, 0);

  child2.pos = A + 0.5*fromA;
  child2.yAxis = Vector2d(fromA[1], -fromA[0]).normalized();
  child2.xAxis = -fromA.normalized();
  child2.pos[0] *= dir; 
  child2.yAxis[0] *= dir; 
  child2.xAxis[1] *= dir;
  child2.length = length;
  child2.width = b;
  child2.width2 = width2;
  child2.children = children;
  child2.dir = -dir;
  child2.peak = peak;

  peak = apex; 
  peak[0] *= dir;
  width2 = d;

  children.clear();
  children.push_back(child1);
  children.push_back(child2);

  for (auto &child : children)
    child.split();
  dir = -dir;
  split();
}

int chapter3Figure5()
{
  gradient = (1.0 - 2.0*pow((1.0 - c) / 2.0, 1.0 / p)) / (0.5*pow(c, 1.0 / p));
  if (gradient < 0.0)
  {
    cout << "gradient less than zero. Choose a different p or c." << endl;
    exit(1);
  }
  Section base;
  base.xAxis = Vector2d(1, 0);
  base.yAxis = Vector2d(0, 1);
  base.length = 1.1;
  base.width = base.length * gradient;
  base.width2 = 0;
  base.pos = Vector2d(0, 0);
  base.dir = 1.0;

  base.split();
  saveSVG("tree.svg", base);
  return 0;
}
