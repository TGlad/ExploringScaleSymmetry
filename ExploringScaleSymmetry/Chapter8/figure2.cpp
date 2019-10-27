// Generates four of the 25 2D classes, the cluster-tree, cluster-cluster, tree-cluster and tree-tree.
#include "stdafx.h"
#include "bmp.h"
#include <fstream>

static int width = 1024;
static int height = 1024;

void putpixel(vector<BYTE> &out, const Vector2i &pos, int shade)
{
  if (pos[0] < 0 || pos[0] >= width || pos[1] < 0 || pos[1] >= width)
    return;
  int ind = 3 * (pos[0] + width*pos[1]);
  out[ind + 0] = out[ind + 1] = out[ind + 2] = shade;
}

void drawDisk(const Vector2d &pos, vector<BYTE> &out, double rad, int shade)
{
  for (int x = (int)(pos[0] - rad); x <= (int)(pos[0] + rad); x++)
    for (int y = (int)(pos[1] - rad); y <= (int)(pos[1] + rad); y++)
      if (sqr(x - pos[0]) + sqr(y - pos[1]) <= sqr(rad))
        putpixel(out, Vector2i(x, y), 255 - shade);
}

struct Node
{
  Vector2d pos;
  double angle;
  Vector2d xAxis() const { return Vector2d(cos(angle), sin(angle)); }
  Vector2d yAxis() const { return Vector2d(-sin(angle), cos(angle)); }
  double radius;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
static int side = 0;

void buildCluster(vector<Node> &cluster, const Node &node, int level, int type)
{
  cluster.push_back(node);
  if (node.radius < 0.5)
    return;

  Vector2d offset(512, 512);
  double scale1 = 0.5;
  Node child1;
  Vector2d vec = node.pos - offset;
  double ang = 0.5; 

  double add1, add2;
  if (type == 0) // cluster-cluster
  {
    add1 = 0.3;
    add2 = 0.3;
  }
  else if (type == 1) // tree-cluster
  {
    add1 = -0.2;
    add2 = -0.2;
    if ((level % 2) == 0)
    {
      if (side == 0)
        add1 = 0.3;
      else
        add2 = 0.3;
      side = (side + 1) % 2;
    }
  }
  else if (type == 2) // cluster-tree
  {
    add1 = 0.0;
    add2 = 0.0;
  }
  else // tree-tree
  {
    add1 = -0.2;
    add2 = -0.2;
  }

  child1.angle = node.angle - 0.5;
  child1.pos = node.pos + child1.yAxis()*node.radius*(1.0 + scale1 + add1);
  child1.radius = node.radius * scale1;
  buildCluster(cluster, child1, level+1, type);

  Node child2;
  child2.angle = node.angle + 0.5;
  child2.pos = node.pos + child2.yAxis()*node.radius*(1.0 + scale1 + add2);
  child2.radius = node.radius * scale1;
  buildCluster(cluster, child2, level+1, type);
}

int _tmain(int argc, _TCHAR* argv[])
{
  for (int i = 0; i < 4; i++)
  {
    Node base;
    base.angle = 3.1415;
    base.radius = 240;
    base.pos = Vector2d(512, 780);

    vector<Node> cluster;
    long s2;
    vector<BYTE> out(width*height * 3); // .bmp pixel buffer
    memset(&out[0], 255, out.size() * sizeof(BYTE)); // background is grey

    buildCluster(cluster, base, 0, i);
    for (auto &planet : cluster)
      drawDisk(planet.pos, out, planet.radius, 255);

    BYTE* c = ConvertRGBToBMPBuffer(&out[0], width, height, &s2);
    LPCTSTR files[] = { L"cluster-cluster.bmp", L"tree-cluster.bmp", L"cluster-tree.bmp", L"tree-tree.bmp" };
    SaveBMP(c, width, height, s2, files[i]);
    delete[] c;
  }
}
