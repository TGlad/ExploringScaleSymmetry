#include "Core3D.h"
#include "Screen.h"
#include <conio.h>
#include <windows.h>

void Core3D::init(int screenWidth, int screenHeight)
{
  screen = new Screen(screenWidth, screenHeight);
  view = new View3D(screenWidth, screenHeight);
}

void Core3D::deinit()
{
  delete view;
}
void Core3D::render()
{
  view->recordToScreen(screen);
}

void Core3D::update(double timeStep)
{
  view->update();
}
