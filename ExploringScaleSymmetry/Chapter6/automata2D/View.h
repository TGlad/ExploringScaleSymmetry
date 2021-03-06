﻿// Thomas Lowe, 2020.
#pragma once
#include "basics.h"
#include "evolver.h"

class View 
{
public:
  View(int width, int height);
  int width;
  int height;
  class Screen* screen; 
  void recordToScreen(Screen* screen);
  void update();
  void setMaster(int m);
  void resetFromHead(char c = 0);
  void load();
  void save();
  
  Evolver* evolvers[7];
};