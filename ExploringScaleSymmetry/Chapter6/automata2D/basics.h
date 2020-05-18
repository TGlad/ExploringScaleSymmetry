// Thomas Lowe, 2020.
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>



#include "Vector3.h"

// TODO: shouldn't be necessary, given templates below
inline float clamped(float value, float left, float right)
{
  return value<left ? left : (value > right ? right : value);
}
inline float random()
{
  return -1.0f + (2.0f * (float)rand() / (float)RAND_MAX);
}
inline float random(float min, float max)
{
  return min + (max-min)*((float)rand() / (float)RAND_MAX);
}
template<class T> inline const T& max(const T& left, const T& right)
{	
	return right>left ? right : left;
}
template<class T> inline const T& min(const T& left, const T& right)
{	
	return right<left ? right : left;
}
template<class T> inline const T& clamped(const T& value, const T& left, const T& right)
{	
  return value<left ? left : (value > right ? right : value);
}
inline float sqr(float a)
{
  return a*a;
}
inline float absf(float a)
{
  int b=(*((int *)(&a)))&0x7FFFFFFF;
  return *((float *)(&b));
}

#include <crtdbg.h> // won't work on linux
#define BREAK() __debugbreak()
#if defined(_DEBUG)
#define ASSERT(exp)  { if(!(exp)) { printf("assert failed: %s", #exp); BREAK(); } }
#else
#define ASSERT(exp) {}
#endif

#include "Matrix33.h"
