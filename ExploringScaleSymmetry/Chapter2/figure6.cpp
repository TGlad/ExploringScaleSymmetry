// Generates the pyramidal surface fractal as a .ply triangle-mesh file. 
#include "stdafx.h"
#include "PolyMesh.h"
double sqr(double x){ return x*x; }

// The interior angle of the triangle, in radians. pi/2 gives a square, and 1.18 is close to the smallest angle before the surface self-intersects 
static double theta = 1.18; 
// if enabled, this just shows the pyramid section of the subdivided triangle
#define JUST_PYRAMID

static double h2 = (1.0 + sqrt(1.0 + sqr(tan(theta)) - sqr(sin(theta))))/(2.0*sqr(1.0/cos(theta))-2.0);
static double h = sqrt(h2);
static double a = sqrt(sqr(sin(theta / 2.0)) + h2);
static double b = sqrt(sqr(cos(theta / 2.0)) + h2);
void subdivide(TriMesh &mesh, const Vector3d &v1, const Vector3d &v2, const Vector3d &v3, bool flip)
{
  double scale = (v2 - v1).norm() / (1.0 + b / a);
  if (scale < 0.01)
  {
    TriMesh::Triangle tri;
    tri.corner[0] = v1;
    tri.corner[1] = v2;
    tri.corner[2] = v3;
    if (flip)
      swap(tri.corner[1], tri.corner[2]);
    mesh.triangles.push_back(tri);
    return;
  }
  // else subdivide
  Vector3d v12 = v1 + (v2 - v1)*(1.0 / (1.0 + b / a));
  Vector3d v13 = v1 + (v3 - v1)*(1.0 / (1.0 + a / b));
  Vector3d v23 = v2 + (v3 - v2)*((1.0 / a) / ((1.0 / a) + (1.0 / b)));
  Vector3d up = (v2 - v1).cross(v3 - v1).normalized();
  Vector3d vh = (v1 + v23) / 2.0 + up*h*scale * (flip ? -1.0 : 1.0);

#if defined JUST_PYRAMID
  if (scale < 1.0-1e-10)
#endif
  {
    subdivide(mesh, v12, v2, v23, flip);
    subdivide(mesh, v13, v23, v3, flip);
  }
  subdivide(mesh, vh, v1, v12, flip);
  subdivide(mesh, vh, v23, v13, flip);
  subdivide(mesh, vh, v23, v12, !flip);
  subdivide(mesh, vh, v1, v13, !flip);
}

int _tmain(int argc, _TCHAR* argv[])
{
  TriMesh mesh;
  Vector3d v1(0, 0, 0);
  Vector3d v2(1.0 + b / a, 0, 0);
  Vector3d v3 = (1.0 + a / b) * Vector3d(cos(theta), sin(theta), 0);
  subdivide(mesh, v1, v2, v3, false);

  mesh.savePLY("pyramidal.ply");
  // mesh.saveOBJ("pyramidal.obj"); // one could also save in this format
  exit(1);
}
