// Calculates the unknown scaling component in the three displayed binary trees. This is the (unnormalised) Minkowski content (c)
// Also generates an image showing the expanded shape
#include "stdafx.h"
#include "bmp.h"
#include <sstream>
#include <fstream>

// Define which shape to extract the Minkowski content from
#define BINARY_TREE1
//#define BINARY_TREE2
//#define BINARY_TREE3

namespace
{
  struct Node
  {
    Vector2d pos;
    Vector2d up;
    double length;
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };
}

static int width = 1024;
static int height = 750;
static Vector2d offset(500.0, 600.0);
static vector<Vector2d> leaves;
static double minLength = 0.0025;
static double area = 0.0;
static Vector2d minVec(1e10, 1e10);
static Vector2d maxVec(-1e10, -1e10);

static const double rt5 = sqrt(0.5);
void recurse(vector<Node, aligned_allocator<Node> > &list, Node &node)
{
  minVec = Vector2d(min(minVec[0], node.pos[0]), min(minVec[1], node.pos[1]));
  maxVec = Vector2d(max(maxVec[0], node.pos[0]), max(maxVec[1], node.pos[1]));
  list.push_back(node);
  if (node.length < minLength)
    return;
  Vector2d side(-node.up[1], node.up[0]);


  Node child;
#if defined BINARY_TREE1
  child.length = node.length * 5.0 / 12.0;
#elif defined BINARY_TREE2
  child.length = node.length * 6.0 / 12.0;
#elif defined BINARY_TREE3
  child.length = node.length * 7.0 / 12.0;
#endif
  Vector2d top = node.pos + node.up*node.length;

  child.pos = top;
  child.up = node.up * rt5 + side*rt5;
  recurse(list, child);
  child.pos = top;
  child.up = node.up * rt5 - side*rt5;
  recurse(list, child);
}

static void putpixel(vector<BYTE> &out, const Vector2i &pos, int shade)
{
  if (pos[0] < 0 || pos[0] >= width || pos[1] < 0 || pos[1] >= height)
    return;
  int ind = 3 * (pos[0] + width*pos[1]);
  out[ind + 0] = out[ind + 1] = out[ind + 2] = shade;
}

int chapter9Figure4()
{
  Node base;
  base.pos = Vector2d(0, 0);
  base.up = Vector2d(0, 1);
  base.length = 1.0;
  vector<Node, aligned_allocator<Node> > list;
  recurse(list, base);

  long s2;
  vector<BYTE> out(width*height * 3); // .bmp pixel buffer
  vector<double> graph;
  double c = 0;
  {
    memset(&out[0], 255, out.size() * sizeof(BYTE)); // background is grey
    double r = 0.1;
    double w = 0.0015; 
    double m = 1.0 / r;
    double minX = minVec[0] - r;
    double minY = minVec[1] - r;
    double maxX = maxVec[0] + r;
    double maxY = maxVec[1] + r;
    double radius2 = r*r;
    double Nmink = 0;
    Vector2d di(sin(22.5*pi / 180.0), cos(22.5*pi / 180.0));
    for (double x = minX; x < maxX; x += w)
    {
      cout << "percentage through: " << 100.0*(x-minX) / (maxX - minX) << endl;
      for (double y = minY; y < maxY; y += w)
      {
        Vector2d p(x, y);
        for (auto &node : list)
        {
          Vector2d nodePos = node.pos;
          Vector2d p(x, y);
          if (y < 0.5 && (y < x || p.dot(di) < 0.0))
            continue;
          Vector2d side(-node.up[1], node.up[0]);
          Vector2d ps[2] = { node.pos, node.pos + node.up*node.length };
          Vector2d dir = ps[1] - ps[0];
          double d = (p - ps[0]).dot(dir) / dir.squaredNorm();
          Vector2d nearest = ps[0] + dir*max(0.0, min(d, 1.0));
          double minDist = (nearest - p).squaredNorm();
          if (minDist < radius2)
          {
            Nmink++;
            double W = 2*w;
            putpixel(out, Vector2i(0.01 + (x - minX) / W, 0.01 + (y - minY) / W), 192);
            break;
          }
        }
      }
    }
    for (auto &node : list)
    {
      double W = 2*w;
      putpixel(out, Vector2i(0.01 + (node.pos[0] - minX) / W, 0.01 + (node.pos[1] - minY) / W), 0);
    }
    Nmink *= w*w; // now a volume
    Nmink *= m*m; // now a Minkowski number
#if defined BINARY_TREE1
    cout << "r scaling process:" << endl;
    double d = log(2.0) / log(12.0 / 5.0);
    for (int i = 0; i < 10; i++)
    {
      Nmink *= 2.0;
      m *= 12.0 / 5.0;
      Nmink += 2.0*m;
      Nmink -= 1.5*tan(22.5*pi/180.0)/(m*m);
      Nmink -= 0.5*tan(45.0*pi / 180.0) / (m*m);
      cout << "m: " << m << ", Nmink: " << Nmink << endl;
      cout << "estimated c: " << (12.0*m - Nmink) / pow(m, d) << endl;
    }
#elif defined BINARY_TREE2
    cout << "r scaling process:" << endl;
    double d = log(2.0) / log(12.0 / 6.0);
    for (int i = 0; i < 10; i++)
    {
      Nmink *= 2.0;
      m *= 12.0 / 6.0;
      Nmink += 2.0*m;
      Nmink -= 1.5*tan(22.5*pi / 180.0) / (m*m);
      Nmink -= 0.5*tan(45.0*pi / 180.0) / (m*m);
      cout << "m: " << m << ", Nmink: " << Nmink << endl;
      double est = m*log(m) * 2.0 / log(2.0);
      cout << "estimated c: " << (Nmink - est) / pow(m, d) << endl;
    }
#elif defined BINARY_TREE3
    cout << "r scaling process:" << endl;
    double d = log(2.0) / log(12.0 / 7.0);
    for (int i = 0; i < 10; i++)
    {
      Nmink *= 2.0;
      m *= 12.0 / 7.0;
      Nmink += 2.0*m;
      Nmink -= 1.5*tan(22.5*pi / 180.0) / (m*m);
      Nmink -= 0.5*tan(45.0*pi / 180.0) / (m*m);
      cout << "m: " << m << ", Nmink: " << Nmink << endl;
      cout << "estimated c: " << (12.0*m + Nmink) / pow(m, d) << endl;
    }
#endif

    BYTE* buf = ConvertRGBToBMPBuffer(&out[0], width, height, &s2);
    LPCTSTR file = L"binary_tree_expanded.bmp";
    SaveBMP(buf, width, height, s2, file);
    delete[] buf;
  }
  return 0;
}
