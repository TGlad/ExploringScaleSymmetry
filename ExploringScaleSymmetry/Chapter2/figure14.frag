#info 2D Line Distance Estimator  // A variant of the Koch curve in 3D space which creates a 2D line.
#define providesInit
#include "DE-Raytracer.frag"
#include "MathUtils.frag"
#group 2D Line

void init() {
}

// Number of fractal iterations.
uniform int Iterations;  slider[0,30,100]

// The distance estimation calculation
float DE(vec3 z)
{
     float r;
     int n = 0;
     vec3 p = z;
     float ang = 3.1415/4.0;
     float len = 1.0/cos(ang);
     float sc = 1.0;
     while (n < Iterations)
     {
          if (p.x > 0.0)
          {
              p.x -= 1.0;
              float p2x = sin(ang)*p.z + cos(ang)*p.x;
              float p2z = cos(ang)*p.z - sin(ang)*p.x;
              p.x = p2x;
              p.z = p2z;
              p.x += len/2.0;

              float y2 = p.z;
              p.z = p.y;
              p.y = -y2;
          }
          else
          {
              p.x += 1.0;
              float p2x = sin(-ang)*p.z + cos(-ang)*p.x;
              float p2z = cos(-ang)*p.z - sin(-ang)*p.x;
              p.x = p2x;
              p.z = p2z;
              p.x -= len/2.0;

              float y2 = p.z;
              p.z = p.y;
              p.y = -y2;
          }
          p *= 2.0/len;
          sc *= 2.0/len;
          n++;
	}
     float k = 1.0;
     if (p.x > k)
        p.x -= k;
     else if (p.x < -k)
        p.x += k;
     else 
        p.x = 0;
     float l = length(p);
     return l / sc;
}

#preset Default
FOV = 0.4
Eye = -0.91199,-2.42234,0.247336
Target = 2.35017,6.98029,-0.726773
Up = -0.914285,0.27406,-0.298287
AntiAlias = 1
Detail = -2.23006
DetailAO = -0.63
FudgeFactor = 0.916
MaxRaySteps = 112
BoundingSphere = 2
Dither = 0.22807
AO = 0,0,0,0.96721
Specular = 1.4167
SpecularExp = 18.8
SpotLight = 1,1,1,0.17391
SpotLightDir = 0.31428,0.1
CamLight = 1,1,1,1.41936
CamLightMin = 0.15294
Glow = 0.835294,0.0784314,0.0784314,0
Fog = 0
HardShadow = 0.13846
Reflection = 0
BaseColor = 1,1,1
OrbitStrength = 0.515
X = 0.6,0.0117647,0.0117647,0.59056
Y = 1,0.6,0,0.44882
Z = 1,1,1,0.49606
R = 0.666667,0.666667,0.498039,0.07936
BackgroundColor = 0.666667,0.666667,0.498039
GradientBackground = 0.3
CycleColors = false
Cycles = 4.27409
FloorNormal = 0,0,0
FloorHeight = 0
FloorColor = 1,1,1
Scale = 2
Offset = 1,0,0
Angle1 = 0
Rot1 = 1,1,1
Angle2 = 0
Rot2 = 1,1,1
Iterations = 13
#endpreset
