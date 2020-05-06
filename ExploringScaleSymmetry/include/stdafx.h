// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include "targetver.h"

int chapter2Figure3();
int chapter2Figure4();
int chapter2Figure6();
int chapter2Figure10();
int chapter2Figure15();
int chapter2Figure16();
int chapter2Figure19();
int chapter3Figure1();
int chapter3Figure5();
int chapter3Figure9();
int chapter3Figure10();
int chapter3Figure12();
int chapter4Figure9();
int chapter4Figure11();
int chapter4Figure20();
int chapter5Figure22();
int chapter5Figure25();
int chapter6Figure12();
int chapter6Figure17();
int chapter7Figure1();
int chapter7Figure3();
int chapter7Figure4();
int chapter7Figure6();
int chapter7Figure7();
int chapter7Figure12();
int chapter7Figure13();
int chapter8Figure2();
int chapter9Figure2();
int chapter9Figure5();
int chapter9Figure12();

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <strstream>
#include <Eigen>
#include <StdVector>
using namespace std;
using namespace Eigen;
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector2d);
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector3d);
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector4d);
const double pi = 3.14159265;
inline double sqr(double x)
{
  return x*x;
}
inline double random(double from, double to)
{
  return from + (to - from)*(rand() % 10000) / 10000.0;
}