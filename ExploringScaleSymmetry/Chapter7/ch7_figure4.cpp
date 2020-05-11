// Generates image of scale-symmetric ocean water. It is a first order approximation of water with low amplitude and depth.
#include "stdafx.h"
#include "bmp.h"
#include <set>

static double seaHeight = -0.1;
static Vector3d seaBase = Vector3d(0.1, 0.25, 0.35);
static Vector3d seaWaterColour = Vector3d(0.8, 0.9, 0.6);


static const int width = 1024;
static const int height = 512;

static void putpixel(vector<BYTE> &out, const Vector2i &pos, const Vector3d &colour)
{
  if (pos[0] < 0 || pos[0] >= width || pos[1] < 0 || pos[1] >= height)
    return;
  int ind = 3 * (pos[0] + width*(height - 1 - pos[1]));
  out[ind + 0] = max(0, min((int)(255.0*colour[0]), 255));
  out[ind + 1] = max(0, min((int)(255.0*colour[1]), 255));
  out[ind + 2] = max(0, min((int)(255.0*colour[2]), 255));
}

static const double PI = 3.14159;

static Vector3d mix(const Vector3d &a, const Vector3d &b, double t)
{
  return a + (b - a)*t;
}

static Vector3d reflect(const Vector3d &ray, const Vector3d &normal)
{
  return ray - normal * 2.0*ray.dot(normal);
}

// lighting
static double diffuse(const Vector3d &n, const Vector3d &l, double p)
{
  return pow(n.dot(l) * 0.4 + 0.6, p);
}
static double specular(const Vector3d &n, const Vector3d &l, const Vector3d &e, double s)
{
  double nrm = (s + 8.0) / (PI * 8.0);
  return pow(max(reflect(e, n).dot(l), 0.0), s) * nrm;
}

// sky
static Vector3d getSkyColor(Vector3d e)
{
  e[1] = max(e[1], 0.0);
  return Vector3d(pow(1.0 - e[1]*0.4, 2.0), 1.0 - e[1]*0.4, 0.8 + (1.0 - e[1])*0.2);
}

// Some approximations of reflections, refractions and the Fresnel reflectivity term. 
static Vector3d getSeaColour(const Vector3d &p, const Vector3d &n, const Vector3d &l, const Vector3d &eye, const Vector3d &dist)
{
  double fresnel = max(0.0, min(1.0 - n.dot(-eye), 1.0));
  fresnel = pow(fresnel, 2.0) * 1.0;

  Vector3d reflected = getSkyColor(reflect(eye, n));
  Vector3d refracted = seaBase + diffuse(n, l, 80.0) * seaWaterColour * 0.12;

  Vector3d color = mix(refracted, reflected, fresnel);

  double atten = max(1.0 - dist.dot(dist) * 0.001, 0.0);
  color += seaWaterColour * max(0.0, (p[1] - seaHeight)) * 0.18 * atten;

  double s = specular(n, l, eye, 60.0);
  color += Vector3d(s,s,s);

  return color;
}

int chapter7Figure4()
{
  long s2;
  vector<BYTE> out(width*height * 3); // .bmp pixel buffer
  memset(&out[0], 255, out.size() * sizeof(BYTE)); 

  Vector3d camPos(0.2, -2.0, 1.0);
  Vector3d dir = Vector3d(-0.05, 0, -0.2) - camPos;
  dir.normalize();
  Vector3d side = Vector3d(0, 0, -1).cross(dir).normalized();
  Vector3d up = side.cross(dir);
  double time = 7.3; 
  double scaler = 0.5;

  for (double y = 1.0; y >= -1.0; y -= 0.0008)
  {
    for (double x = -1.0; x <= 1.0; x += 0.001)
    {
      double amp = 0.3;
      double wavelength = 1.0;
      double yaw = 0.25;
      double h = 0;
      Vector2d grad(0, 0);
      for (int i = 0; i < 10; i++)
      {
        double ang = -0.25*pi + 0.5*pi*yaw; // this keeps the wave direction within a 90 degree window, to mimic wind-driven waves
        Vector2d waveDir(sin(ang), cos(ang));
        double phase = (x*waveDir[0] + y*waveDir[1] + time) / wavelength;
        h += amp*sin(phase);
        grad += waveDir * amp * cos(phase) / wavelength;

        amp *= scaler;
        wavelength *= scaler;
        yaw = fmod(yaw + 1.618, 1.0); // rather than actual random wave directions, I add an irrational number each time, to avoid clustering
      }

      // convert the point to screen space
      Vector3d pos(x, y, h);
      pos -= camPos;
      Vector3d viewPos;
      viewPos[0] = pos.dot(side);
      viewPos[1] = pos.dot(dir);
      viewPos[2] = pos.dot(up);
      Vector2d screenPos = Vector2d(viewPos[0] / viewPos[1], viewPos[2] / viewPos[1]) * (double)width / 1.5;
      screenPos += Vector2d(width / 2, height / 2);

      // lighting
      Vector3d normal = Vector3d(-grad[0], -grad[1], 1.0).normalized();
      Vector3d ray = pos.normalized();
      Vector3d sun = Vector3d(0.0, 2.0, 1.6).normalized();

      Vector3d colour = getSeaColour(Vector3d(x,y,h), normal, sun, ray, pos);

      putpixel(out, Vector2i(screenPos[0], screenPos[1]), colour);
    }
  }

  BYTE* c = ConvertRGBToBMPBuffer(&out[0], width, height, &s2);
  LPCTSTR file = L"ocean_water.bmp";
  SaveBMP(c, width, height, s2, file);
  delete[] c;
  return 0;
}

