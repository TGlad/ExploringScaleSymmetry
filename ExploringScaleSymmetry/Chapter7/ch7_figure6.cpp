// Thomas Lowe, 2020.
// Generates a vector graphic (.svg) of a spacetime scale-symmetric tree at a specified time instant
#include "stdafx.h"
#include <fstream>

// The tree parameters
static double timeT = 248.0;                                   // the rendered time point. At 0, the result shows spatial symmetry
static double widthPerLength = 0.2;                           // affects the thickness of the tree
static double omega = 2.0;

namespace
{
  struct SSBranch
  {
    Vector2d pos;
    double angle;
    Vector2d xAxis() const { return Vector2d(cos(angle), sin(angle)); }
    Vector2d yAxis() const { return Vector2d(-sin(angle), cos(angle)); }
    double length;
    bool flip;
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };
}

static double scale = 750.0;
static Vector2d offset(0.6, 0);

static void saveSVG(const string &fileName, const vector<SSBranch, aligned_allocator<SSBranch> > &tree)
{
  static ofstream svg;
  svg.open(fileName.c_str());
  svg << "<svg width = \"" << (int)scale << "\" height = \"" << (int)scale << "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;

  for (auto &node : tree)
  {
    Vector2d start = node.pos;
    Vector2d x = node.xAxis() * node.length * widthPerLength * 0.5;
    Vector2d y = node.yAxis() * node.length * 1.1;

    Vector2d corners[] = { start - x, start + x, start + 0.8*x + y, start - 0.8*x + y };
    double gscale = 0.9*scale;
    svg << "<path d = \"M " << gscale*(corners[3][0] + offset[0]) << " " << scale - gscale*(corners[3][1] + offset[1]);
    for (int i = 0; i < 4; i++)
      svg << " L " << gscale*(corners[i][0] + offset[0]) << " " << scale - gscale*(corners[i][1] + offset[1]);
    svg << "\" fill=\"black\" stroke-width = \"1\" stroke=\"black\" />\n";
  }

  svg << "</svg>" << endl;
  svg.close();
}

// recursive generation of branches
void buildTree(vector<SSBranch, aligned_allocator<SSBranch> > &tree, const SSBranch &node)
{
  tree.push_back(node);
  if (node.length < 0.005)
    return;

  double scale1 = 0.75; double scale2 = 0.53828;  // relative size of child branch 1 and 2
  double angle1 = -0.4; double angle2 = 0.6;      // relative angle of child branch 1 and 2
  double sway = 0.25;                             // amplitude of angular oscillation of branches
  if (node.flip)
  {
    angle1 *= -1;
    angle2 *= -1;
    swap(angle1, angle2);
    swap(scale1, scale2);
    sway = -sway;
  }

  SSBranch child1;
  child1.pos = node.pos + node.yAxis() * node.length;
  child1.length = node.length * scale1;
  child1.angle = node.angle + angle1 + sway * sin(timeT / child1.length);
  child1.flip = !node.flip;
  buildTree(tree, child1);

  SSBranch child2;
  child2.pos = node.pos + node.yAxis() * node.length;
  child2.length = node.length * scale2;
  child2.angle = node.angle + angle2 + sway * sin(timeT / child2.length);
  child2.flip = !node.flip;
  buildTree(tree, child2);
}

int chapter7Figure6()
{
  SSBranch base;
  base.angle = 0.2;
  base.length = 0.28;
  base.pos = Vector2d(0, 0);
  base.flip = false;

  vector<SSBranch, aligned_allocator<SSBranch> > tree;
  buildTree(tree, base);

  saveSVG("swaying_tree.svg", tree);
  return 0;
}

