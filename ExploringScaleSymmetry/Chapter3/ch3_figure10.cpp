// Thomas Lowe, 2020.
// Generates a recursive rings structure. A form of multiplcation rule.
#include "stdafx.h"
#include "bmp.h"
#include <fstream>

// Controls detail level
static double minLength = 0.5;
namespace
{
  struct RingNode
  {
    Vector2d pos;
    double radius;
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };
}

static double scale = 750.0;

static void saveSVG(const string &fileName, const vector<RingNode, Eigen::aligned_allocator<RingNode> > &list)
{
  static ofstream svg;
  svg.open(fileName.c_str());
  svg << "<svg width = \"" << (int)scale << "\" height = \"" << (int)scale << "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;

  for (int i = 0; i < (int)list.size(); i++)
  {
    double r2 = list[i].radius*1.1; 
    double r1 = list[i].radius*0.9;
    svg << "<circle cx = \"" << list[i].pos[0] << "\" cy = \"" << list[i].pos[1] << "\" r = \"" << r2 << "\" fill=\"black\" />" << endl;
    svg << "<circle cx = \"" << list[i].pos[0] << "\" cy = \"" << list[i].pos[1] << "\" r = \"" << r1 << "\" fill=\"white\" />" << endl;
  }
  svg << "</svg>" << endl;
  svg.close();
}

static void split(vector<RingNode, Eigen::aligned_allocator<RingNode> > &list, const RingNode &node)
{
  list.push_back(node);
  if (node.radius <= minLength)
  {
    return;
  }
  RingNode child1;
  double s = 1.151;
  child1.pos = node.pos + s*Vector2d(sqrt(3.0) / 2.0, 0.5)*node.radius;
  child1.radius = node.radius / 3.0;
  split(list, child1);
  RingNode child2;
  child2.pos = node.pos + s*Vector2d(-sqrt(3.0) / 2.0, 0.5)*node.radius;
  child2.radius = node.radius / 3.0;
  split(list, child2);
  RingNode child3;
  child3.pos = node.pos + s*Vector2d(0, -1.0)*node.radius;
  child3.radius = node.radius / 3.0;
  split(list, child3);
}

int chapter3Figure10()
{
  RingNode base;
  base.pos = Vector2d(scale/2.0, 50 + scale/2.0);
  base.radius = 200.0;
  vector<RingNode, Eigen::aligned_allocator<RingNode> > list;
  split(list, base);

  saveSVG("recursive_rings.svg", list);
  return 0;
}
