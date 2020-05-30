// Thomas Lowe, 2020.
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
int chapter3Figure17();
int chapter4Figure9();
int chapter4Figure11();
int chapter4Figure20();
int chapter5Figure22();
int chapter5Figure25();
int chapter6Figure3();
int chapter6Figure12();
int chapter6Figure17();
int chapter6Figure19();
int chapter7Figure1();
int chapter7Figure3();
int chapter7Figure4();
int chapter7Figure6();
int chapter7Figure7();
int chapter7Figure12();
int chapter7Figure13();
int chapter8Figure2();
int chapter9Figure2();
int chapter9Figure4();
int chapter9Figure5();
int chapter9Figure12();
int chapter9Figure12_disktree();
int chapter9Figure12_forddisks();
int chapter9Figure12_spheretree();

#include <stdio.h>
#include <iostream>
#include <vector>
#include <strstream>
#ifdef _WIN32
#include <windows.h>
#include <Eigen>
#include <StdVector>
#else
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/StdVector>
typedef unsigned char BYTE;
#endif

using namespace Eigen;
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector2d);
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector3d);
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector4d);
using namespace std;
const double pi = 3.14159265;
inline double sqr(double x)
{
  return x*x;
}
inline double random(double from, double to)
{
  return from + (to - from)*(rand() % 10000) / 10000.0;
}
