// Generates one of the cross family of structures. 
#include <fstream>
#include "stdafx.h"
#include "bmp.h"

// Change this parameter for cross structures with different numbers of sides. 
static int sides = 5;
// Controls the detail level
static int numIterations = 7;
namespace
{
  struct Section
  {
    Vector2d pos;
    Vector2d up;
    void expand();
    void draw(ofstream &svg, const Vector2d &origin);
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };
}

static double scale = 750.0;
static Vector2d offset(1.0, 0.5);
static vector<Vector2d> tri;

static void saveSVG(const string &fileName, const vector<Section> &list)
{
  static ofstream svg;
  svg.open(fileName.c_str());
  svg << "<svg width = \"" << (int)(2.0*scale) << "\" height = \"" << (int)scale << "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;

  for (auto &sec : list)
  {
    Vector2d side(sec.up[1], -sec.up[0]);
    for (int i = 0; i < (int)tri.size(); i++)
    {
      Vector2d end = sec.pos + tri[i][0] * side + tri[i][1] * sec.up;
      svg << "<path d = \"M " << scale*(sec.pos[0] + offset[0]) << " " << scale - scale*(sec.pos[1] + offset[1]);
      svg << " L " << scale*(end[0] + offset[0]) << " " << scale - scale*(end[1] + offset[1]);
      svg << "\" stroke=\"black\" stroke-width=\"1\" />\n"; 
    }
  }

  svg << "</svg>" << endl;
  svg.close();
}

static vector<Section> transform(vector<Section> &list, const Vector2d &translation)
{
  vector<Section> newChild = list;
  for (int i = 0; i < (int)newChild.size(); i++)
  {
    newChild[i].pos = -newChild[i].pos + translation;
    newChild[i].up = -newChild[i].up;
  }
  return newChild;
}

int chapter3Figure12()
{
  for (int i = 0; i < sides; i++)
  {
    double angle = (double)i * 2.0*pi / (double)sides;
    tri.push_back(2.0 * Vector2d(sin(angle), cos(angle)));
  }
  vector<Section> list(1);
  list[0].pos = Vector2d(0, 0);
  double scl = 0.005;
  list[0].up = Vector2d(0, scl);
  vector<Vector2d> ends(sides);
  for (int i = 0; i<sides; i++)
    ends[i] = tri[i] * scl;
  for (int j = 0; j < numIterations; j++)
  {
    vector<Section> newList;
    vector<Vector2d> newEnds(sides);
    for (int i = 0; i < sides; i++)
    {
      vector<Section> newChild = transform(list, ends[i]);
      newList.insert(newList.end(), newChild.begin(), newChild.end());
      newEnds[i] = -ends[(i + (sides/2)) % sides] + ends[i];
    }
    list = newList;
    for (int i = 0; i < sides; i++)
      ends[i] = newEnds[i];
  }
  if (sides == 3)
    saveSVG("fudgeflake.svg", list);
  else if (sides == 4)
    saveSVG("greekcross.svg", list);
  else if (sides == 5)
    saveSVG("pentacross.svg", list);
  else if (sides == 6)
    saveSVG("hexacross.svg", list);
  return 0;
}
