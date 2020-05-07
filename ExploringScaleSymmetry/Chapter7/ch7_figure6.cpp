// Generates a vector graphic (.svg) of a spacetime scale-symmetric tree at a specified time instant
#include "stdafx.h"
#include "bmp.h"
#include <fstream>

// The tree parameters
static double time = 248.0;                                   // the rendered time point. At 0, the result shows spatial symmetry
static double widthPerLength = 0.2;                           // affects the thickness of the tree
static double scale1 = 0.75; static double scale2 = 0.53828;  // relative size of child branch 1 and 2
static double angle1 = -0.4; static double angle2 = 0.6;      // relative angle of child branch 1 and 2
static double sway = 0.25;                                    // amplitude of angular oscillation of branches


struct Node
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

static double scale = 750.0;
static Vector2d offset(0.6, 0);

inline void saveSVG(const string &fileName, const vector<Node> &tree)
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
void buildTree(vector<Node> &tree, const Node &node)
{
  tree.push_back(node);
  if (node.length < 0.005)
    return;

  if (node.flip)
  {
    angle1 *= -1;
    angle2 *= -1;
    swap(angle1, angle2);
    swap(scale1, scale2);
    sway = -sway;
  }

  Node child1;
  child1.pos = node.pos + node.yAxis() * node.length;
  child1.length = node.length * scale1;
  child1.angle = node.angle + angle1 + sway * sin(time / child1.length);
  child1.flip = !node.flip;
  buildTree(tree, child1);

  Node child2;
  child2.pos = node.pos + node.yAxis() * node.length;
  child2.length = node.length * scale2;
  child2.angle = node.angle + angle2 + sway * sin(time / child1.length);
  child2.flip = !node.flip;
  buildTree(tree, child2);
}

int chapter7Figure6()
{
  Node base;
  base.angle = 0.2;
  base.length = 0.28;
  base.pos = Vector2d(0, 0);
  base.flip = false;

  vector<Node> tree;
  buildTree(tree, base);

  saveSVG("scale_symmetric_tree.svg", tree);
  return 0;
}
