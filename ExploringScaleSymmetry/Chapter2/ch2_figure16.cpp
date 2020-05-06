// Generates the 2.5 dimensional square-based fractal surface, as a .ply mesh file. 
#include "stdafx.h"
#include "PolyMesh.h"

// Controls the minimum resolution:
static double minimumLength = 0.01;

void subdiv(TriMesh &mesh, const Vector3d &v, Vector3d d1, Vector3d d2)
{
  double scale = d1.norm();
  if (scale < minimumLength)
  {
    TriMesh::Triangle tri;
    tri.corner[0] = v;
    tri.corner[1] = v+d1;
    tri.corner[2] = v+d2;
    mesh.triangles.push_back(tri);
    tri.corner[0] = v + d1;
    tri.corner[1] = v + d1 + d2;
    tri.corner[2] = v + d2;
    mesh.triangles.push_back(tri);
    return;
  }
  Vector3d up = d1.cross(d2).normalized() * scale / 4.0;
  d1 /= 4.0;
  d2 /= 4.0;

  // bottom 12 faces
  subdiv(mesh, v, d1, d2);
  subdiv(mesh, v+d1, d1, d2);
  subdiv(mesh, v+2*d1, d2, d1);
  subdiv(mesh, v+3*d1, d1, d2);
  subdiv(mesh, v + d2, d1, d2);
  subdiv(mesh, v + 2 * d2, d2, d1);
  subdiv(mesh, v + 3 * d1 + d2, d2, d1);
  subdiv(mesh, v + 3 * d1 + 2 * d2, d1, d2);
  subdiv(mesh, v + 3 * d2, d1, d2);
  subdiv(mesh, v + d1 + 3 * d2, d2, d1);
  subdiv(mesh, v + 2 * d1 + 3 * d2, d1, d2);
  subdiv(mesh, v + 3 * d1 + 3 * d2, d1, d2);

  // top face
  subdiv(mesh, v + d1 + d2 + 2*up, d1, d2);
  subdiv(mesh, v + d1 + 2 * d2 + 2 * up, d1, d2);
  subdiv(mesh, v + 2 * d1 + d2 + 2 * up, d1, d2);
  subdiv(mesh, v + 2 * d1 + 2 * d2 + 2 * up, d1, d2);

  // front side
  subdiv(mesh, v + d1 + d2, up, d2);
  subdiv(mesh, v + d1 + 2 * d2, d2, up);
  subdiv(mesh, v + d1 + d2 + up, up, d2);
  subdiv(mesh, v + d1 + 2 * d2 + up, up, d2);

  // left side
  subdiv(mesh, v + d1 + d2, up, d1);
  subdiv(mesh, v + 2 * d1 + d2, d1, up);
  subdiv(mesh, v + d1 + d2 + up, d1, up);
  subdiv(mesh, v + 2 * d1 + d2 + up, d1, up);

  // right side
  subdiv(mesh, v + d1 + 3*d2, up, d1);
  subdiv(mesh, v + 2 * d1 + 3 * d2, d1, up);
  subdiv(mesh, v + d1 + 3 * d2 + up, up, d1);
  subdiv(mesh, v + 2 * d1 + 3 * d2 + up, up, d1);

  // back side
  subdiv(mesh, v + 3 * d1 + d2, up, d2);
  subdiv(mesh, v + 3 * d1 + 2 * d2, d2, up);
  subdiv(mesh, v + 3 * d1 + d2 + up, d2, up);
  subdiv(mesh, v + 3 * d1 + 2 * d2 + up, d2, up);
}

int chapter2Figure16()
{
  TriMesh mesh;
  Vector3d v(0, 0, 0);
  Vector3d d1(1, 0, 0);
  Vector3d d2(0, 1, 0);
  subdiv(mesh, v, d1, d2);

  mesh.savePLY("square_surface.ply");
  return 0;
}

