#info Ford Spheres Distance Estimator.
#define providesInit
#include "DE-Raytracer.frag"
#include "MathUtils.frag"
#group Ford Spheres

uniform int Iterations;  slider[0,10,100]
uniform float k; slider[0.00,0.5,1.00]

void init() {
}

// the distance estimation function
float DE(vec3 p)
{
     float rad = k;
     vec3 s0 = vec3(0.0,2.0/sqrt(3.0),rad);
     vec3 s1 = vec3( 1.0,-1.0/sqrt(3.0),rad);
     vec3 s2 = vec3(-1.0,-1.0/sqrt(3.0),rad);
     vec3 t0 = vec3(0.0,2.0/sqrt(3.0),0.0);
     vec3 t1 = vec3( 1.0,-1.0/sqrt(3.0),0.0);
     vec3 t2 = vec3(-1.0,-1.0/sqrt(3.0),0.0);
     float norm = sqrt(3.0)/2.0;
     vec3 n0 = vec3(1.0,0.0,0.0);
     vec3 n1 = vec3(1.0/sqrt(3.0),1.0,0.0) * norm;
     vec3 n2 = vec3(1.0/sqrt(3.0),-1.0,0.0) * norm;

     float scale = rad; 
     int numIterations = Iterations;
     if (p.z > 1.0*rad)
       numIterations = 1;
     for (int i = 0; i < numIterations; i++)
     {
        // first modolu the space, so it has hexagonal symmetry...
        float x = dot(p-t2, t1)*0.5;
        float y = dot(p-t2, t0)*0.5;
        x = mod(x, 1.0);
        y = mod(y, 1.0);
        if (x+y > 1.0)
        {
          x = 1 - x;
          y = 1 - y;
        }
        float h = p.z;
        p = t2 + x*(t1-t2) + y*(t0-t2);
        p.z = h;
        if (numIterations != 1)
        {
          float dot0 = dot(p, n0);
          float dot1 = dot(p, n1);
          float dot2 = dot(p, n2);
          float s = 1.0;
          // put into correct tridrant
          if (dot1 < 0.0 && dot2 > 0.0)
          {
            // first invert
            vec3 fromT0 = p - t0;
            s = dot(fromT0, fromT0);
            p = t0 + fromT0 * 4.0/s;
            // then reflect
            vec3 fromT2 = p - t2;
            p = t2 + fromT2 - 2.0 * t0 * dot(fromT2, t0) * 3.0/4.0;
          }  
          else if (dot0 > 0 && dot1 > 0)
          {
            // first invert
            vec3 fromT2 = p - t2;
		  s = dot(fromT2, fromT2);
            p = t2 + fromT2 * 4.0/s;
            // then reflect
            vec3 fromT1 = p - t1;
            p = t1 + fromT1 - 2.0 * t2 * dot(fromT1, t2) * 3.0/4.0;
          }
          else
          {
            // first invert
            vec3 fromT1 = p - t1;
            s = dot(fromT1, fromT1);
            p = t1 + fromT1 *4.0/s;
            // then reflect
            vec3 fromT2 = p - t2;
            p = t2 + fromT2 - 2.0 * t1 * dot(fromT2, t1) * 3.0/4.0;
          }	
          scale *= s/4.0;
        }
     }
     float d0 =length(p-s0) - rad;
     float d1 = length(p-s1) - rad;
     float d2 = length(p-s2) - rad;
     float d = min(d0, min(d1, d2)) * scale;
		
     return d;
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
k = 0.5
#endpreset
