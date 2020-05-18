#pragma once
#include "basics.h"
#include "View3D.h"
#include "Screen.h"

class Core3D
{
public:
  void init(int screenWidth, int screenHeight);
  void deinit();
  void render();
  void update(double timeStep);
  Screen* screen;
  View3D* view;
};
