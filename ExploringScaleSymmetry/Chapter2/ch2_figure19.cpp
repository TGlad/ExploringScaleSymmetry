// Thomas Lowe, 2020.
// Generates a member of the dendrite family. A tridendrite, Vicsek fractal, Pentadendrite or a hexaflake
#include "stdafx.h"
#include "bmp.h"
#include <fstream>

// Set this value to change the number of sides
const int sides = 3;

namespace
{
  struct FlakeSection
  {
    Vector2d pos;
    Vector2d up;
    void expand();
    void draw(ofstream &svg, const Vector2d &origin);
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };
}

static double scale = 760.0;
static Vector2d offset(1.0, 0.5);
static vector<Vector2d> shape;

static void saveSVG(const string &fileName, const vector<FlakeSection, Eigen::aligned_allocator<FlakeSection> > &list)
{
  static ofstream svg;
  svg.open(fileName.c_str());
  svg << "<svg width = \"" << (int)(2.0*scale) << "\" height = \"" << (int)scale << "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;

  for (auto &sec : list)
  {
    Vector2d side(sec.up[1], -sec.up[0]);
    for (int i = 0; i < (int)shape.size(); i++)
    {
      Vector2d end = sec.pos + shape[i][0] * side + shape[i][1] * sec.up;
      svg << "<path d = \"M " << scale*(sec.pos[0] + offset[0]) << " " << scale - scale*(sec.pos[1] + offset[1]);
      svg << " L " << scale*(end[0] + offset[0]) << " " << scale - scale*(end[1] + offset[1]);
      svg << "\" stroke=\"black\" stroke-width=\"1\" />\n"; 
    }
  }

  svg << "</svg>" << endl;
  svg.close();
}

static vector<FlakeSection, Eigen::aligned_allocator<FlakeSection> > transform(vector<FlakeSection, Eigen::aligned_allocator<FlakeSection> > &list, const Vector2d &translation)
{
  vector<FlakeSection, Eigen::aligned_allocator<FlakeSection> > newChild = list;
  for (int i = 0; i < (int)newChild.size(); i++)
  {
    newChild[i].pos = -newChild[i].pos + translation;
    newChild[i].up = -newChild[i].up;
  }
  return newChild;
}

int chapter2Figure19()
{
  for (int i = 0; i < sides; i++)
  {
    double angle = (double)i * 2.0*pi / (double)sides;
    shape.push_back(2.0 * Vector2d(sin(angle), cos(angle)));
  }
  vector<FlakeSection, Eigen::aligned_allocator<FlakeSection> > list(1);
  list[0].pos = Vector2d(0, 0);
  double scl = 0.001; 
  list[0].up = Vector2d(0, scl);
  vector<Vector2d> ends(sides);
  for (int i = 0; i<sides; i++)
    ends[i] = shape[i] * scl;

  for (int j = 0; j < 5; j++)
  {
    vector<FlakeSection, Eigen::aligned_allocator<FlakeSection> > newList;
    vector<Vector2d> newEnds(sides);
    for (int i = 0; i < sides; i++)
    {
      vector<FlakeSection, Eigen::aligned_allocator<FlakeSection> > newChild = transform(list, 2.0*ends[i]);
      newList.insert(newList.end(), newChild.begin(), newChild.end());
      newEnds[i] = -ends[(i + (sides/2)) % sides] + 2.0*ends[i];
    }
    list.insert(list.end(), newList.begin(), newList.end());
    for (int i = 0; i < sides; i++)
      ends[i] = newEnds[i];
  }
  if (sides == 3)
    saveSVG("tridendrite.svg", list);
  else if (sides == 4)
    saveSVG("vicsek.svg", list);
  else if (sides == 5)
    saveSVG("pentadendrite.svg", list);
  else if (sides == 6)
    saveSVG("hexaflake.svg", list);
  return 0;
}
