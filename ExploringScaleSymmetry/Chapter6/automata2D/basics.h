// Thomas Lowe, 2020.
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>



#include "Vector3.h"

// TODO: shouldn't be necessary, given templates below
inline float clamped(float value, float left, float right)
{
  return value<left ? left : (value > right ? right : value);
}
/*inline float random()
{
  return -1.0f + (2.0f * (float)rand() / (float)RAND_MAX);
}*/
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
#define ASSERT(exp){ if (!(exp)){ std::cout << "assertion failure: " << #exp << std::endl; }}

#ifdef _WIN32
#include <conio.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

inline bool _kbhit()
{
  termios term;
  tcgetattr(0, &term);

  termios term2 = term;
  term2.c_lflag &= ~ICANON;
  tcsetattr(0, TCSANOW, &term2);

  int byteswaiting;
  ioctl(0, FIONREAD, &byteswaiting);

  tcsetattr(0, TCSANOW, &term);

  return byteswaiting > 0;
}

inline char _getch() 
{
  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0)
          perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
          perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0)
          perror ("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
          perror ("tcsetattr ~ICANON");
  return (buf);
}
#endif
#ifdef _WIN32
const char enterKey = 13;
#else
const char enterKey = 10;
#endif

#include "Matrix33.h"
