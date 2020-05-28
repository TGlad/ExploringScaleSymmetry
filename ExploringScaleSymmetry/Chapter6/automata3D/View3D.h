#pragma once
#include "basics.h"
#include "evolver3D.h"

class View3D 
{
public:
  View3D(int width, int height);
  int width;
  int height;
  class Screen* screen; 
  void recordToScreen(Screen* screen);
  void update();
  void setMaster(int m);
  void resetFromHead(int type);
  void load();
  void save();
  void setToLetter(char letter);
  
  Evolver3D* evolvers[7];
  Evolver3D* bigEvolver; // is allocated and deallocated on the fly
};