#info classification table for 3D structures
#define providesInside
#define providesColor
#include "Brute-Raytracer.frag"
#group Classification table

// Number of fractal iterations.
uniform int Iterations;  slider[0,4,100]

vec3 color(vec3 p) {
	return abs(vec3(p));
}

bool inside(vec3 pos) {
     int b000 = 0;
     int b100 = 0;
     int b200 = 0;
     int b110 = 0;
     int b210 = 0;
     int b220 = 0;
     int b111 = 0;
     int b211 = 0;
     int b221 = 0;
     int b222 = 0;
     float scale = 5.0;
     bool drawBorder = false;
     bool classificationList = false; 
     
 //    if (pos.z > 0.3333) // option to cut through to see inside
 ///      return false;
     pos.x += 3.5;
     pos.y += 3.5;
     pos *= 0.9;
     float tx = floor(pos.x);
     float ty = floor(pos.y);
     pos.x -= tx;
     pos.y -= ty;
     pos /= 0.9;
     pos.x -= 0.5;
     pos.y -= 0.5;
     if (classificationList)
     { 
       if (tx < 3.0 || tx >= 4.0 || ty < 0.0 || ty > 6.0)
         return false;
       tx = 6-ty;
     }
     else if (tx < 0.0 || tx > 6.0 || ty < 0.0 || ty > 6.0)
       return false;
     int type = 10 * int(tx) + 6-int(ty);

     // implement a 3x3 or 5x5 cell replacement scheme
     // for each structure. bxyz are the cell index locations relative to the centre
     // e.g. b000 is the centre cell, b111 is the corner if it is a 3x3x3 grid, or b222 is the corner if it is a 5x5x5 grid.
     // the values mean 0: empty, 1: subdivide, 2: solid
     if (type == 00) // void-void
     {}
     if (type == 01) // void-cluster
     { b111 = 1; scale = 3.0; drawBorder = true; }
     if (type == 02) // void-tree
     { b000 = 1; b100 = 1; scale = 3.0; drawBorder = true; }
     if (type == 03) // void-sponge
     { b110 = 1; b111 = 1; scale = 3.0; drawBorder = true; }
     if (type == 04) // void-shell
     { b000 = 1; b100 = 1; b110 = 1; scale = 3.0; drawBorder = true; }
     if (type == 05) // void-foam
     { b100 = 1; b110 = 1; b111 = 1; scale = 3.0; drawBorder = true; }
     if (type == 06) // void-solid
     { b000 = 2; b100 = 2; b110 = 2; b111 = 2; scale = 3.0; drawBorder = true; }
     if (type == 10) // cluster-cluster
     { b000 = 2; b200 = 1; b220 = 1; b222 = 1; }
     if (type == 11) // cluster-cluster
     { b000 = 2; b200 = 1; b220 = 1; b222 = 1; }
     if (type == 12) // cluster-tree
     { b000 = 2; b100 = 1; scale = 3.0; }
     if (type == 13) // cluster-sponge
     { b000 = 2; b100=2; b111 = 1; scale = 3.0; }
     if (type == 14) // cluster-shell
     { b000 = 2; b100 = 1; b110 = 1; scale = 3.0;}
     if (type == 15) // cluster-foam
     { b100 = 2; b110 = 1; b111 = 1; scale = 3.0;}
     if (type == 16) // cluster-solid
     { b000 = 2; b110 = 1; b111 = 1; b100 = 1; scale = 3.0;}
     if (type == 20) // tree-void
     { b000 = 2; b100 = 2; b200 = 2; b210 = 1; }
     if (type == 21) // tree-cluster
     { b000 = 2; b100 = 2; b200 = 2; b210 = 1; b222 = 1;}
     if (type == 22) // tree-tree
     { b000 = 2; b100 = 2; b200 = 2; b210 = 1;}
     if (type == 23) // tree-sponge
     { b111 = 2; b110 = 1; scale = 3.0; }
     if (type == 24) // tree-shell
     { b000 = 2; b100 = 2; b110 = 1; scale = 3.0; }
     if (type == 25) // tree-foam
     { b111 = 2; b100 = 1; b110 = 1; scale = 3.0;}
     if (type == 26) // tree-solid
     { b111=2;  b000 = 1; b100 = 1; b110 = 1; scale = 3.0; }
     if (type == 30) // sponge-void
     { b000 = 2; b100 = 2; b200 = 2; b110 = 1; b210 = 1; b111 = 1; b211 = 1; }
     if (type == 31) // sponge-cluster
     { b220 = 2; b221 = 2; b222 = 2; b210 = 1; b211 = 1; b000 = 1;} 
     if (type == 32) // sponge-tree
     { b000 = 2; b100 = 2; b200 = 2; b110 = 1; b111 = 1; b211 = 1; }
      if (type == 33) // sponge-sponge
     { b000 = 2; b100 = 2; b200 = 2; b110 = 1; b210 = 1; b111 = 1; b211 = 1; }
     if (type == 34) // sponge-shell
     { b000 = 2; b100 = 2; b110 = 1; scale = 3.0; }
     if (type == 35) // sponge-foam
     { b110 = 2; b111 = 1; scale = 3.0; }
     if (type == 36) // sponge-solid
     { b000 = 2; b100 = 2; b110 = 1; b111 = 1; scale = 3.0; }
     if (type == 40)  // shell-void
     { b110 = 2; b111 = 2; b211 = 2; b220 = 2; b221 = 2; b222 = 2; b210 = 1; }
     if (type == 41) // shell-cluster
     { b000 = 2; b100 = 2; b200 = 2; b110 = 2; b210 = 2; b220 = 1; b222 = 1; } // more precisely a sponge-shell-cluster
     if (type == 42) // shell-tree
     { scale = 3.0; pos.z += 0.29;  pos /= 1.8; }
     if (type == 43) // shell-sponge
     { b210 = 2; b220 = 2; b221 = 2; b222 = 2; b110 = 1; b111 = 1; b211 = 1; } 
     if (type == 44) // shell-shell
     { b110 = 2; b111 = 2; b211 = 2; b220 = 2; b221 = 2; b222 = 2; b210 = 1; }
     if (type == 45) // shell-foam
     { b111 = 2; b110 = 2; b100 = 1; scale = 3.0;}
     if (type == 46) // shell-solid
     { b110 = 2; b111 = 2; b000 = 1; b100 = 1; scale = 3.0; }
     if (type == 50) // foam-void
     { b000 = 2; b100 = 2; b110 = 2; b200 = 2; b210 = 2; b220 = 2; b221 = 2; b222 = 2; b211 = 1; }
     if (type == 51) // foam-cluster
     { b000 = 1; b200 = 1; b210 = 1; b220 = 1; b211 = 1; b221 = 1; b222 = 1; scale = 7.0;}
     if (type == 52) // foam-tree
     { b100 = 2; b200 = 2; b210 = 1; }
     if (type == 53) // foam-sponge
     { b210 = 2; b220 = 2; b211 = 2; b221 = 2; b110 = 1; b222 = 1; b111 = 1; } // mor precisely a shell-foam-sponge
     if (type == 54) // foam-shell
     { b211 = 2; b221 = 2; b220 = 2; b111 = 2; b110 = 2; b210 = 1; b222 = 1; }
     if (type == 55) // foam-foam
     { b200 = 2; b210 = 2; b220 = 2; b211 = 2; b221 = 2; b222 = 2; b100 = 1; b110 = 1; b111 = 1;}
     if (type == 56) // foam-solid
     { b000 = 2; b100 = 2; b110 = 2; b111 = 1; scale = 3.0;}
     if (type == 60) // solid-void
     { b000 = 2; b100 = 2; b200 = 2; b110 = 2; b210= 2; b220 = 2; b111 = 2; b211 = 2; b221 = 2; b222 = 2;}
     if (type == 61) // solid-cluster
     { b111 = 2; b211 = 1;  }
     if (type == 62) // solid-tree
     { b000 = 2; b100 = 2; b200 = 2; b210 = 1;}
     if (type == 63) // solid-sponge
     { b000 = 2; b100 = 2; b200 = 2; b110 = 1; b210 = 1; b111 = 1; b211 = 1; }
     if (type == 64) // solid-shell
     { b110 = 2; b111 = 2; b211 = 2; b220 = 2; b221 = 2; b222 = 2; b210 = 1; }
     if (type == 65) // solid-foam
     { b200 = 2; b210 = 2; b220 = 2; b211 = 2; b221 = 2; b222 = 2; b100 = 1; b110 = 1; b111 = 1;}
     if (type == 66) // solid-solid
     { b000 = 2; b100 = 2; b200 = 2; b110 = 2; b210= 2; b220 = 2; b111 = 2; b211 = 2; b221 = 2; b222 = 2;}

     if (ty == 6.0)
     {
        if (pos.y > 0.0)
          return false;
        pos.y += 0.5;
     }
     else if (tx == 6.0)
     {
        if (pos.y < 0.0 && pos.x < 0.5 && pos.z < 0.5 && pos.z > -0.5)
          return true;
        if (pos.y > 0.5)
          return false;
        pos.y -= 0.5;
     }	

     vec3 p = pos * scale;
     bool flip = false;

     for (int i=0; i<Iterations; i++) 
     {
        p.x = abs(p.x);
        p.y = abs(p.y);
        if (type == 42)
        {
           if (p.x > 1.5 || p.y > 1.5 || p.z > 1.0 || p.z < 0.0)
              return flip;
           if (p.x > 0.5)
           {
              float t = p.z;
              p.z = p.x - 0.5;
              p.x = t - 0.5;
           }
           else if (p.y > 0.5)
           {
              float t = p.z;
              p.z = p.y - 0.5;
              p.y = t - 0.5;
           }
           else
           {
              flip = !flip;
              p.z = 1.0 - p.z;
           }
           p *= scale;
           continue;
        }
        p.z = abs(p.z);
        // get them in order 
        if (p.z > p.y && p.z > p.x)
        {
          pos.x = p.z;
          if (p.x > p.y)
            pos.y = p.x, pos.z = p.y;
          else 
            pos.y = p.y, pos.z = p.x;
        }
        else if (p.y > p.z && p.y > p.x)
        {
          pos.x = p.y;
          if (p.x > p.z)
            pos.y = p.x, pos.z = p.z;
          else 
            pos.y = p.z, pos.z = p.x;
        }
        else
        {
          pos.x = p.x;
          if (p.y > p.z)
            pos.y = p.y, pos.z = p.z;
          else 
            pos.y = p.z, pos.z = p.y;
        }
	pos += vec3(0.5,0.5,0.5);
        float indx = floor(pos.x);
        float indy = floor(pos.y);
        float indz = floor(pos.z);
        pos -= vec3(indx, indy, indz);
	pos -= vec3(0.5,0.5,0.5);

        if (indx == 0 && indy == 0 && indz == 0)
        {  if (b000 == 2) return true; else if (b000 == 0) return false; }
        else if (indx == 1 && indy == 0 && indz == 0)
        {  if (b100 == 2) return true; else if (b100 == 0) return false; }
        else if (indx == 2 && indy == 0 && indz == 0)
        {  if (b200 == 2) return true; else if (b200 == 0) return false; }
        else if (indx == 1 && indy == 1 && indz == 0)
        {  if (b110 == 2) return true; else if (b110 == 0) return false; }
        else if (indx == 2 && indy == 1 && indz == 0)
        {  if (b210 == 2) return true; else if (b210 == 0) return false; }
        else if (indx == 2 && indy == 2 && indz == 0)
        {  if (b220 == 2) return true; else if (b220 == 0) return false; }
        else if (indx == 1 && indy == 1 && indz == 1)
        {  if (b111 == 2) return true; else if (b111 == 0) return false; }
        else if (indx == 2 && indy == 1 && indz == 1)
        {  if (b211 == 2) return true; else if (b211 == 0) return false; }
        else if (indx == 2 && indy == 2 && indz == 1)
        {  if (b221 == 2) return true; else if (b221 == 0) return false; }
        else if (indx == 2 && indy == 2 && indz == 2)
        {  if (b222 == 2) return true; else if (b222 == 0) return false; }
        else if (type ==51)
        {
           return indx<=3 && indy<=3 && indz<=3; 
        }
        else
           return false;
        if (scale == 3 && (indx==2 || indy==2 || indz==2))
           return false;
        p = pos * scale;
     }
     return drawBorder;
}

#preset Default
FOV = 0.62536
Eye = 0.0160772,0.048,6.34273
Target = 0,0.048,0
Up = 0,1,0
EquiRectangular = false
Gamma = 2.5
ToneMapping = 1
Exposure = 1.34694
Brightness = 1
Contrast = 0.9901
Saturation = 1
NormalScale = 1.05265
AOScale = 5
Glow = 0
AOStrength = 0.86047
Samples = 100
Stratify = true
DebugInside = false
CentralDifferences = true
SampleNeighbors = true
Near = 5.52636
Far = 8.27328
ShowDepth = false
DebugNormals = false
Specular = 0.4651
SpecularExp = 5.455
SpotLight = 1,0.678431,0.494118,0.78431
SpotLightDir = 0.35714,0.78126
CamLight = 1,1,1,0.46602
CamLightMin = 0
Fog = 0
BaseColor = 1,1,1
OrbitStrength = 0
X = 1,1,1,1
Y = 0.345098,0.666667,0,0.02912
Z = 1,0.666667,0,1
R = 0.0784314,1,0.941176,-0.0194
BackgroundColor = 1,1,1
GradientBackground = 0
CycleColors = false
Cycles = 1.1
Iterations = 4
#endpreset
