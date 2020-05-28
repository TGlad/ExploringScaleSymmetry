// Thomas Lowe, 2020.
#pragma once
#include "basics.h"
#include "GL/glut.h"

struct ScreenColour
{
  GLubyte red;
  GLubyte green;
  GLubyte blue;
  GLubyte alpha;
  inline ScreenColour(const Vector3& colour)
  {
    red = (GLubyte)(255.0f * colour.x);
    green = (GLubyte)(255.0f * colour.y);
    blue = (GLubyte)(255.0f * colour.z);
    alpha = 255;
  }
  inline ScreenColour(){}
};
