﻿// Thomas Lowe, 2020.
#include "Core.h"
#include "Screen.h"

void Core::init(int screenWidth, int screenHeight)
{
  screen = new Screen(screenWidth, screenHeight);
  view = new View(screenWidth, screenHeight);
}

void Core::deinit()
{
  delete view;
}
void Core::render()
{
  view->recordToScreen(screen);
}

void Core::update(double timeStep)
{
  view->update();
}
