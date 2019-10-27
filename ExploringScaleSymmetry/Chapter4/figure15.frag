#info Bubbles Edge Distance Estimator.
#define providesInit
#define providesBackground
#include "DE-Raytracer-Bubbles.frag"
#include "MathUtils.frag"
#group Bubbles Edge

uniform int Iterations;  slider[0,10,100]
uniform float k; slider[0.00,3.0,6.00]

void init() {
}

// ground and sky for context
vec3 backgroundColor(vec3 dir)
{
  vec3 col = vec3(0.1, 0.2, 1.0);
  if (dir.z > 0.0)
  {
     col = vec3(0.7,0.8,1.0);
     col += (vec3(0.1,0.3,1.0) - col)*min(dir.z*dir.z*1.5, 1.0);
  }
  else
  {
     col = vec3(0.15,0.40,0.12);
     col += (vec3(0.1,0.2,0.02) - col)*min(dir.z*dir.z*2.5, 1.0);
  } 
  if (dir.z > 0.99)
  {
    col  += (1.5*vec3(1.0, 1.0, 0.88) - col)*max((dir.z-0.99)*300.0, 0.0);
  }
  return col;
}

// the distance estimation function
float DE(vec3 p)
{
     vec3 v0 = vec3(0.0, 0.0, 1.0);
     vec3 v1 = vec3(sqrt(8.0/9.0), 0, -1.0/3.0 );
     vec3 v2 = vec3(-sqrt(2.0/9.0), sqrt(2.0/3.0), -1.0/3.0 );
     vec3 v3 = vec3(-sqrt(2.0/9.0), -sqrt(2.0/3.0), -1.0/3.0 );

     float dScale = 1.0;
     for (int i = 0; i < Iterations; i++)
     {
        float bendDist = 2.0 + 2.0/3.0;
        float l0 = dot(p - v0, p - v0);
        float l1 = dot(p - v1, p - v1);
        float l2 = dot(p - v2, p - v2);
        float l3 = dot(p - v3, p - v3);
        vec3 v = v3;
        if (l0 < l1 && l0 < l2 && l0 < l3)
          v = v0;
        else if (l1 < l0 && l1 < l2 && l1 < l3)
          v = v1;
        else if (l2 < l0 && l2 < l1 && l2 < l3)
          v = v2;

        p += v * (bendDist - 1.0);
        float s = bendDist*bendDist / dot(p,p);
        p *= s;
        dScale *= s;
        p -= v * (dot(p,v) * 2.0 - bendDist);
        p *= k;
        dScale *= k;
     }

     // find distance to nearest edge of radial tetrahedron
     float l0 = dot(p - v0, p - v0);
     float l1 = dot(p - v1, p - v1);
     float l2 = dot(p - v2, p - v2);
     float l3 = dot(p - v3, p - v3);
     vec3 v = v3;
     if (l0 < l1 && l0 < l2 && l0 < l3)
       v = v0;
     else if (l1 < l0 && l1 < l2 && l1 < l3)
       v = v1;
     else if (l2 < l0 && l2 < l1 && l2 < l3)
       v = v2;
     float t = dot(p, v);
     float d = length(p - t*v) / dScale;
	
     return 0.5*d;
}


#preset Default
FOV = 0.4
Eye = -3.72729,-0.0860174,-1.93389
Target = 5.14721,0.118786,2.6706
Up = -0.00486875,0.999305,-0.0369503
Detail = -2.84956
DetailAO = -1.33
FudgeFactor = 1
MaxRaySteps = 96
Dither = 0.5
AO = 0,0,0,0.7
Specular = 0.1666
SpecularExp = 16
SpotLight = 1,1,1,0.03261
SpotLightDir = 0.37142,0.1
CamLight = 1,1,1,1.13978
CamLightMin = 0.61176
Glow = 1,1,1,0.05479
Fog = 0.25926
HardShadow = 0
Reflection = 0
BaseColor = 1,1,1
OrbitStrength = 0
X = 0.5,0.6,0.6,0.2126
Y = 1,0.6,0,0.30708
Z = 0.8,0.78,1,0.35434
R = 0.666667,0.666667,0.498039,0.03174
BackgroundColor = 0.278431,0.513725,0.6
GradientBackground = 0.4348
CycleColors = false
Cycles = 1.1
FloorNormal = 0,0,0
FloorHeight = 0
FloorColor = 1,1,1
Iterations = 10
k = 3.0
#endpreset
