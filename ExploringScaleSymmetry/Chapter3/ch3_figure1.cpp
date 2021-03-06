// Thomas Lowe, 2020.
// Fractal canopies
#include "stdafx.h"
#include <fstream>
static int type = 0; // 0 = thick tree, 1 = thin tree, 2 = tree leaves

static double gradient = type == 0 ? 0.15 : 0.0001;
namespace
{
  struct TreeNode
  {
    Vector2d pos, peak;
    Vector2d xAxis, yAxis;
    double width, width2, length;
    double dir;
    vector<TreeNode, Eigen::aligned_allocator<TreeNode> > children;
    void split();
    void draw(ofstream &svg, const Vector2d &origin, const Vector2d &xAx, const Vector2d &yAx);
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };
}
double scale = 750.0;
Vector2d offset(0.55, 0);
static vector<Vector2d> leaves;
static double minLength = 0.01;

void TreeNode::draw(ofstream &svg, const Vector2d &origin, const Vector2d &xAx, const Vector2d &yAx)
{
  Vector2d start = origin + xAx*pos[0] + yAx*pos[1];
  Vector2d x = xAx*xAxis[0] + yAx*xAxis[1];
  Vector2d y = xAx*yAxis[0] + yAx*yAxis[1];

  Vector2d corners[] = { start - x*width*0.5, start + x*width*0.5, start + x*0.5*width2 + y*length, start + x*peak[0] + y*(peak[1] + length), start - x*0.5*width2 + y*length };
  double gscale = 0.9*scale;
  svg << "<path d = \"M " << gscale*(corners[4][0] + offset[0]) << " " << scale-gscale*(corners[4][1] + offset[1]);
  for (int i = 0; i < 5; i++)
    svg << " L " << gscale*(corners[i][0] + offset[0]) << " " << scale-gscale*(corners[i][1] + offset[1]);
  svg << "\" fill=\"grey\" stroke-width = \"1\" stroke=\"grey\" />\n";

  svg << "<path d = \"M " << gscale*(corners[4][0] + offset[0]) << " " << scale - gscale*(corners[4][1] + offset[1]);
  for (int i = 0; i < 1; i++)
    svg << " L " << gscale*(corners[i][0] + offset[0]) << " " << scale - gscale*(corners[i][1] + offset[1]);
  svg << "\" fill=\"none\" stroke-width = \"2\" stroke=\"black\" />\n";
  svg << "<path d = \"M " << gscale*(corners[1][0] + offset[0]) << " " << scale - gscale*(corners[1][1] + offset[1]);
  for (int i = 2; i < 3; i++)
    svg << " L " << gscale*(corners[i][0] + offset[0]) << " " << scale - gscale*(corners[i][1] + offset[1]);
  svg << "\" fill=\"none\" stroke-width = \"2\" stroke=\"black\" />\n";

  Vector2d p = start + y*length;
  for (auto &c : children)
    c.draw(svg, p, x, y);
  if (children.empty())
    leaves.push_back(corners[2]); // pos is in local space!
}

static void saveSVGLeaves(const string &fileName)
{
  static ofstream svg;
  svg.open(fileName.c_str());
  svg << "<svg width = \"" << (int)scale << "\" height = \"" << (int)scale << "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;
  double gscale = 0.9*scale;

  for (auto &p : leaves)
    svg << "<circle cx = \"" << gscale*(p[0] + offset[0]) << "\" cy = \"" << scale - gscale*(p[1] + offset[1]) << "\" r = \"2\" stroke = \"black\" stroke-width = \"1\" fill = \"black\" />" << endl;
  svg << "</svg>" << endl;
  svg.close();
}
static void saveSVG(const string &fileName, TreeNode &tree)
{
  static ofstream svg;
  svg.open(fileName.c_str());
  svg << "<svg width = \"" << (int)scale << "\" height = \"" << (int)scale << "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;

  tree.draw(svg, Vector2d(0, 0), Vector2d(1, 0), Vector2d(0, 1));
  svg << "</svg>" << endl;
  svg.close();
}
static const double cantorScale = 0.3333;

void TreeNode::split()
{
  if (length <= minLength)
  {
    return;
  }
  TreeNode child1, child2;

  double areaLoss = (sqr(width) - sqr(width2)) * cantorScale;
  double c = sqrt((sqr(width)+sqr(width2)+areaLoss)/2.0);
  double a = sqrt(areaLoss);
  double b = sqrt(sqr(c) - areaLoss);
  double theta = atan2(a, b);
  Vector2d p(c*0.5 - b*cos(theta), b*sin(theta));
  Vector2d B(-c/2.0, 0);
  Vector2d A(c/2.0, 0);
  Vector2d fromB = p-B;
  Vector2d fromA = p-A;
  length *= 0.5;// sqrt((1.0 - cantorScale) / 2.0);

  child1.pos = B + 0.5*fromB;
  child1.yAxis = Vector2d(-fromB[1], fromB[0]);
  child1.xAxis = fromB;
  child1.pos[0] *= dir; child1.yAxis[0] *= dir; child1.xAxis[1] *= dir;
  if (child1.xAxis.norm()> 0)
    child1.xAxis.normalize();
  if (child1.yAxis.norm() > 0)
    child1.yAxis.normalize();
  child1.length = length;
  child1.width = a;
  child1.width2 = 0;
  child1.dir = -dir; // remove negation for curly
  child1.peak = Vector2d(0, 0);

  child2.pos = A + 0.5*fromA;
  child2.yAxis = Vector2d(fromA[1], -fromA[0]);
  child2.xAxis = -fromA;
  child2.pos[0] *= dir; child2.yAxis[0] *= dir; child2.xAxis[1] *= dir;
  if (child2.xAxis.norm()>0)
    child2.xAxis.normalize();
  if (child2.yAxis.norm()>0)
    child2.yAxis.normalize();
  child2.length = length;
  child2.width = b;
  child2.width2 = width2;
  child2.children = children;
  child2.dir = -dir;
  child2.peak = peak;

  peak = p; //Vector2d(0, 0);// // use 0 to show right angles triangles
  peak[0] *= dir;
  width2 = c;

  bool hasChildren = children.size() > 0;
  children.clear();
  children.push_back(child1);
  children.push_back(child2);

  for (auto &c : children)
    c.split();
  dir = -dir;
}

int chapter3Figure1()
{
  TreeNode base;
  base.xAxis = Vector2d(1, 0);
  base.yAxis = Vector2d(0, 1);
  base.length = 1.05;
  base.width = base.length * gradient; //  / rawLength;
  base.width2 = 0;
  base.pos = Vector2d(0, 0);
  base.dir = 1.0;

  base.split();

  if (type == 0)
    saveSVG("widetree.svg", base);
  else if (type == 1)
    saveSVG("thintree.svg", base);
  else if (type == 2)
    saveSVGLeaves("treeleaves.svg");
  return 0;
}
