// Thomas Lowe, 2020.
// Generates image of a planetary system of orbiting black holes. The star has two planets, which each have two moons etc.
#include "stdafx.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "imagewrite.h"
#include <fstream>

static double orbitalTime = 315;
static double scale1 = 1.0 / 3.41828; static double scale2 = 1.0 / 4.317;  // relative radius of child planets 1 and 2
static double orbitalDist1 = 3.0; static double orbitalDist2 = 6.315;      // relative distance of child planets 1 and 2

static const int width = 1024;
static const int height = 1024;

static void putpixel(vector<BYTE> &out, const Vector2i &pos, int shade)
{
  if (pos[0] < 0 || pos[0] >= width || pos[1] < 0 || pos[1] >= width)
    return;
  int ind = 3 * (pos[0] + width*pos[1]);
  out[ind + 0] = out[ind + 1] = out[ind + 2] = shade;
}

static void drawDisk(const Vector2d &pos, vector<BYTE> &out, double rad, int shade)
{
  for (int x = (int)(pos[0] - rad); x <= (int)(pos[0] + rad); x++)
    for (int y = (int)(pos[1] - rad); y <= (int)(pos[1] + rad); y++)
      if (sqr(x - pos[0]) + sqr(y - pos[1]) <= sqr(rad))
        putpixel(out, Vector2i(x, y), 255 - shade);
}
namespace
{
  struct Planet
  {
    Vector2d pos;
    double angle;
    Vector2d xAxis() const { return Vector2d(cos(angle), sin(angle)); }
    Vector2d yAxis() const { return Vector2d(-sin(angle), cos(angle)); }
    double radius;
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  };
}

static double timeT = 0.0;

// recursive construction of child planets
static void buildCluster(vector<Planet, aligned_allocator<Planet> > &cluster, const Planet &node)
{
  cluster.push_back(node);
  if (node.radius < 1.0)
    return;

  Planet child1;
  child1.angle = timeT / (node.radius * orbitalDist1);
  child1.pos = node.pos + child1.yAxis() * node.radius * orbitalDist1;
  child1.radius = node.radius * scale1;
  buildCluster(cluster, child1);

  Planet child2;
  child2.angle = timeT / (node.radius * orbitalDist2);
  child2.pos = node.pos + child2.yAxis() * node.radius * orbitalDist2;
  child2.radius = node.radius * scale2;
  buildCluster(cluster, child2);
}

int chapter7Figure7()
{
  Planet base;
  base.angle = 0;
  base.radius = 150;
  base.pos = Vector2d(700, -50);

  vector<Planet, aligned_allocator<Planet> > cluster;

  
  vector<BYTE> out(width*height * 3); // .bmp pixel buffer
  memset(&out[0], 255, out.size() * sizeof(BYTE)); 

  for (int shade = 0; shade < 256; shade += 1)
  {
    timeT = orbitalTime + 125.0 * ((double)shade / 255.0 - 1.0);
    cluster.clear();
    buildCluster(cluster, base);
    for (auto &planet : cluster)
    {
      if (shade == 255)
        drawDisk(planet.pos, out, planet.radius, 255);
      else
        drawDisk(planet.pos, out, planet.radius / 2.0, min(shade, 127));
    }
  }

  stbi_write_png("black_hole_planetary_system.png", width, height, 3, &out[0], 3 * width);
  return 0;
}
