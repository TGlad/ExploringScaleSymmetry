#include "evolver3D.h"
#include <memory.h>
#include <algorithm>
// Code related to specific rule types
using namespace std;

// type 1 
bool Evolver3D::getStatic1(int level, int parentLevel, int x, int y, int z)
{
  int dirX = x % 2 ? 1 : -1;
  int xParent = x >> 1;

  int dirY = y % 2 ? 1 : -1;
  int yParent = y >> 1;

  int dirZ = z % 2 ? 1 : -1;
  int zParent = z >> 1;

  int numParents = getNumInQuad(parentLevel, xParent - ((x + 1) & 1), yParent - ((y + 1) & 1), zParent - ((z + 1) & 1), grids[parentLevel]) << 3;
  if (numParents == 0)
    return false;
  if (numParents == 64)
    return true;
  if (get(parentLevel, xParent, yParent, zParent))
    numParents += 3 * 8; // so 5 points for centre voxel

  for (int kk = 0; kk<2; kk++)
  {
    for (int jj = 0; jj<2; jj++)
    {
      numParents += 4 * (int)get(parentLevel, xParent - dirX, yParent + jj*dirY, zParent + kk*dirZ);
      numParents += 4 * (int)get(parentLevel, xParent + jj*dirX, yParent - dirY, zParent + kk*dirZ);
      numParents += 4 * (int)get(parentLevel, xParent + jj*dirX, yParent + kk*dirY, zParent - dirZ);
    }
    numParents += 2 * (int)get(parentLevel, xParent + kk*dirX, yParent - dirY, zParent - dirZ);
    numParents += 2 * (int)get(parentLevel, xParent - dirX, yParent + kk*dirY, zParent - dirZ);
    numParents += 2 * (int)get(parentLevel, xParent - dirX, yParent - dirY, zParent + kk*dirZ);
  }

  int total = (numParents + 2) >> 3; // rounded to nearest. 0 to 19
  if (total <= 3)
    return false;
  else if (total >= 16)
    return true;
  if (total < 10)
    return genome[total];
  else
    return !genome[15 - total];
}

// type 1
bool Evolver3D::getStatic2(int level, int parentLevel, int x, int y, int z)
{
  int dirX = x % 2 ? 1 : -1;
  int xParent = x >> 1;

  int dirY = y % 2 ? 1 : -1;
  int yParent = y >> 1;

  int dirZ = z % 2 ? 1 : -1;
  int zParent = z >> 1;

  int nearest = (int)get(parentLevel, xParent, yParent, zParent);
  int second = (int)get(parentLevel, xParent + dirX, yParent, zParent) +
    (int)get(parentLevel, xParent, yParent + dirY, zParent) +
    (int)get(parentLevel, xParent, yParent, zParent + dirZ);
  int third = (int)get(parentLevel, xParent + dirX, yParent + dirY, zParent) +
    (int)get(parentLevel, xParent + dirX, yParent, zParent + dirZ) +
    (int)get(parentLevel, xParent, yParent + dirY, zParent + dirZ);
  int fourth = (int)get(parentLevel, xParent + dirX, yParent + dirY, zParent + dirZ);
  int numParents = nearest + second + third + fourth;
  // Note- these two are mandatory
  if (numParents <= 2)
    return false;
  else if (numParents >= 6)
    return true;

  return genome[nearest * 32 + second * 8 + third * 4 + fourth];
}

// type 1
bool Evolver3D::getStatic3(int level, int parentLevel, int x, int y, int z)
{
  int dirX = x % 2 ? 1 : -1;
  int xParent = x >> 1;

  int dirY = y % 2 ? 1 : -1;
  int yParent = y >> 1;

  int dirZ = z % 2 ? 1 : -1;
  int zParent = z >> 1;

  int nearest = (int)get(parentLevel, xParent, yParent, zParent);
  int second = (int)get(parentLevel, xParent + dirX, yParent, zParent) +
    (int)get(parentLevel, xParent, yParent + dirY, zParent) +
    (int)get(parentLevel, xParent, yParent, zParent + dirZ);
  int third = (int)get(parentLevel, xParent + dirX, yParent + dirY, zParent) +
    (int)get(parentLevel, xParent + dirX, yParent, zParent + dirZ) +
    (int)get(parentLevel, xParent, yParent + dirY, zParent + dirZ);
  int fourth = get(parentLevel, xParent + dirX, yParent + dirY, zParent + dirZ) != 0;
  int numParents = nearest + second + third + fourth;
  // Note- these two are mandatory
  if (numParents <= 1)
    return false;
  else if (numParents >= 7)
    return true;

  //   int sixth = (int)get(parentLevel, xParent-dirX, yParent+dirY, zParent) + 
  //               (int)get(parentLevel, xParent-dirX, yParent, zParent+dirZ) + 
  //               (int)get(parentLevel, xParent+dirX, yParent-dirY, zParent) + 
  //               (int)get(parentLevel, xParent, yParent-dirY, zParent+dirZ) + 
  //               (int)get(parentLevel, xParent+dirX, yParent, zParent-dirZ) +
  //               (int)get(parentLevel, xParent, yParent+dirY, zParent-dirZ);
  int seventh = (int)get(parentLevel, xParent - dirX, yParent, zParent) +
    (int)get(parentLevel, xParent, yParent - dirY, zParent) +
    (int)get(parentLevel, xParent, yParent, zParent - dirZ);
  if (numParents + seventh <= 3)
    return false;
  else if (numParents + seventh >= 8)
    return false;

  return genome[(nearest * 32 + second * 8 + third * 4) * 4 + seventh];
}


bool Evolver3D::getStatic4(int level, int parentLevel, int x, int y, int z)
{
  int dirX = x % 2 ? 1 : -1;
  int xParent = x >> 1;

  int dirY = y % 2 ? 1 : -1;
  int yParent = y >> 1;

  int dirZ = z % 2 ? 1 : -1;
  int zParent = z >> 1;

  int nearest = (int)get(parentLevel, xParent, yParent, zParent);
  int second = (int)get(parentLevel, xParent + dirX, yParent, zParent) +
    (int)get(parentLevel, xParent, yParent + dirY, zParent) +
    (int)get(parentLevel, xParent, yParent, zParent + dirZ);
  int third = (int)get(parentLevel, xParent + dirX, yParent + dirY, zParent) +
    (int)get(parentLevel, xParent + dirX, yParent, zParent + dirZ) +
    (int)get(parentLevel, xParent, yParent + dirY, zParent + dirZ);
  int fourth = get(parentLevel, xParent + dirX, yParent + dirY, zParent + dirZ) != 0;
  int numParents = nearest + second + third + fourth;
  // Note- these two are mandatory
  if (numParents <= 1)
    return false;
  else if (numParents >= 7)
    return true;

  int fifth = (int)get(parentLevel, xParent - dirX, yParent + dirY, zParent + dirZ) +
    (int)get(parentLevel, xParent + dirX, yParent - dirY, zParent + dirZ) +
    (int)get(parentLevel, xParent + dirX, yParent + dirZ, zParent - dirZ);
  int sixth = (int)get(parentLevel, xParent - dirX, yParent + dirY, zParent) +
    (int)get(parentLevel, xParent - dirX, yParent, zParent + dirZ) +
    (int)get(parentLevel, xParent + dirX, yParent - dirY, zParent) +
    (int)get(parentLevel, xParent, yParent - dirY, zParent + dirZ) +
    (int)get(parentLevel, xParent + dirX, yParent, zParent - dirZ) +
    (int)get(parentLevel, xParent, yParent + dirY, zParent - dirZ);
  int seventh = (int)get(parentLevel, xParent - dirX, yParent, zParent) +
    (int)get(parentLevel, xParent, yParent - dirY, zParent) +
    (int)get(parentLevel, xParent, yParent, zParent - dirZ);
  int totalParents = numParents + fifth + sixth + seventh; // total = 20 (will never be actually 20, or 0)

  return genome[totalParents];
}

bool Evolver3D::getStatic5(int level, int parentLevel, int x, int y, int z)
{
  int dirX = (x % 2) ? 1 : -1;
  int xParent = x >> 1;

  int dirY = (y % 2) ? 1 : -1;
  int yParent = y >> 1;

  int dirZ = (z % 2) ? 1 : -1;
  int zParent = z >> 1;

  int nearest = (int)get(parentLevel, xParent, yParent, zParent);
  int s1 = (int)get(parentLevel, xParent + dirX, yParent, zParent);
  int s2 = (int)get(parentLevel, xParent, yParent + dirY, zParent);
  int s3 = (int)get(parentLevel, xParent, yParent, zParent + dirZ);
  int second = s1 + s2 + s3;
  int t1 = (int)get(parentLevel, xParent, yParent + dirY, zParent + dirZ);
  int t2 = (int)get(parentLevel, xParent + dirX, yParent, zParent + dirZ);
  int t3 = (int)get(parentLevel, xParent + dirX, yParent + dirY, zParent);
  int third = t1 + t2 + t3;

  int v1 = (int)get(parentLevel, xParent - dirX, yParent, zParent);
  int v2 = (int)get(parentLevel, xParent, yParent - dirY, zParent);
  int v3 = (int)get(parentLevel, xParent, yParent, zParent - dirZ);
  int seventh = v1 + v2 + v3;
  int totalParents = nearest + second + third + seventh; // total = 10
  if (totalParents <= 3)
    return false;
  if (totalParents >= 7)
    return true;

  int pattern1 = (s1 << 0) + (s2 << 1) + (s3 << 2) + (t1 << 3) + (t2 << 4) + (t3 << 5) + (v1 << 6) + (v2 << 7) + (v3 << 8);
  int pattern2 = (s2 << 0) + (s3 << 1) + (s1 << 2) + (t2 << 3) + (t3 << 4) + (t1 << 5) + (v2 << 6) + (v3 << 7) + (v1 << 8);
  int pattern3 = (s3 << 0) + (s1 << 1) + (s2 << 2) + (t3 << 3) + (t1 << 4) + (t2 << 5) + (v3 << 6) + (v1 << 7) + (v2 << 8);
  int pattern4 = (s1 << 0) + (s3 << 1) + (s2 << 2) + (t1 << 3) + (t3 << 4) + (t2 << 5) + (v1 << 6) + (v3 << 7) + (v2 << 8);
  int pattern5 = (s3 << 0) + (s2 << 1) + (s1 << 2) + (t3 << 3) + (t2 << 4) + (t1 << 5) + (v3 << 6) + (v2 << 7) + (v1 << 8);
  int pattern6 = (s2 << 0) + (s1 << 1) + (s3 << 2) + (t2 << 3) + (t1 << 4) + (t3 << 5) + (v2 << 6) + (v1 << 7) + (v3 << 8);
  int pattern = min(min(min(min(min(pattern1, pattern2), pattern3), pattern4), pattern5), pattern6);
  int total = nearest + 2 * pattern; // max 1023
  ASSERT(total >= 0 && total <= 1023);
  if (total <= 511)
    return genome[total];
  else
    return !genome[1023 - total]; // bit symmetry
}

bool Evolver3D::getStatic6(int level, int parentLevel, int x, int y, int z)
{
  int dirX = x % 2 ? 1 : -1;
  int xParent = x >> 1;

  int dirY = y % 2 ? 1 : -1;
  int yParent = y >> 1;

  int dirZ = z % 2 ? 1 : -1;
  int zParent = z >> 1;

  int nearest = (int)get(parentLevel, xParent, yParent, zParent);
  int s1 = (int)get(parentLevel, xParent + dirX, yParent, zParent);
  int s2 = (int)get(parentLevel, xParent, yParent + dirY, zParent);
  int s3 = (int)get(parentLevel, xParent, yParent, zParent + dirZ);
  int second = s1 + s2 + s3;
  int t1 = (int)get(parentLevel, xParent, yParent + dirY, zParent + dirZ);
  int t2 = (int)get(parentLevel, xParent + dirX, yParent, zParent + dirZ);
  int t3 = (int)get(parentLevel, xParent + dirX, yParent + dirY, zParent);
  int third = t1 + t2 + t3;
  int fourth = get(parentLevel, xParent + dirX, yParent + dirY, zParent + dirZ) != 0;

  int v1 = (int)get(parentLevel, xParent - dirX, yParent, zParent);
  int v2 = (int)get(parentLevel, xParent, yParent - dirY, zParent);
  int v3 = (int)get(parentLevel, xParent, yParent, zParent - dirZ);
  int seventh = v1 + v2 + v3;
  int totalParents = nearest + second + third + fourth + seventh; // total = 11
  if (totalParents <= 3)
    return false;
  if (totalParents >= 8)
    return true;

  int pattern1 = (s1 << 0) + (s2 << 1) + (s3 << 2) + (t1 << 3) + (t2 << 4) + (t3 << 5) + (v1 << 6) + (v2 << 7) + (v3 << 8);
  int pattern2 = (s2 << 0) + (s3 << 1) + (s1 << 2) + (t2 << 3) + (t3 << 4) + (t1 << 5) + (v2 << 6) + (v3 << 7) + (v1 << 8);
  int pattern3 = (s3 << 0) + (s1 << 1) + (s2 << 2) + (t3 << 3) + (t1 << 4) + (t2 << 5) + (v3 << 6) + (v1 << 7) + (v2 << 8);
  int pattern4 = (s1 << 0) + (s3 << 1) + (s2 << 2) + (t1 << 3) + (t3 << 4) + (t2 << 5) + (v1 << 6) + (v3 << 7) + (v2 << 8);
  int pattern5 = (s3 << 0) + (s2 << 1) + (s1 << 2) + (t3 << 3) + (t2 << 4) + (t1 << 5) + (v3 << 6) + (v2 << 7) + (v1 << 8);
  int pattern6 = (s2 << 0) + (s1 << 1) + (s3 << 2) + (t2 << 3) + (t1 << 4) + (t3 << 5) + (v2 << 6) + (v1 << 7) + (v3 << 8);
  int pattern = min(min(min(min(min(pattern1, pattern2), pattern3), pattern4), pattern5), pattern6);
  int total = nearest + 2 * fourth + 4 * pattern; // max 2047
  if (total <= 1023)
    return genome[total];
  else
    return !genome[2047 - total]; // bit symmetry
}

// This one is asymmetric in height, which is the z axis
bool Evolver3D::getStatic7(int level, int parentLevel, int x, int y, int z)
{
  int dirX = x % 2 ? 1 : -1;
  int xParent = x >> 1;
  int dirY = y % 2 ? 1 : -1;
  int yParent = y >> 1;
  int dirZ = z % 2 ? 1 : -1;
  int zParent = z >> 1;
  int numParents = 0;

  // idea, use just the adjacent edges, so 7 voxels in total
  // then apply horizontal square symmetry and bit symmetry to get the numbers down
  int nearest = (int)get(parentLevel, xParent, yParent, zParent);
  int a1 = (int)get(parentLevel, xParent + dirX, yParent, zParent);
  int a2 = (int)get(parentLevel, xParent, yParent + dirY, zParent);
  int a3 = (int)get(parentLevel, xParent, yParent, zParent + dirZ);
  int sum = nearest + a1 + a2 + a3;
  if (sum < 1)
    return false;
  if (sum > 3)
    return true;
  int s1 = (int)get(parentLevel, xParent - dirX, yParent, zParent);
  int s2 = (int)get(parentLevel, xParent, yParent - dirY, zParent);
  int s3 = (int)get(parentLevel, xParent, yParent, zParent - dirZ);
  int pattern1 = nearest + (a1 << 1) + (a2 << 2) + (a3 << 3) + (s1 << 4) + (s2 << 5) + (s3 << 6);
  int pattern2 = yUp ? nearest + (a3 << 1) + (a2 << 2) + (a1 << 3) + (s3 << 4) + (s2 << 5) + (s1 << 6)
    : nearest + (a2 << 1) + (a1 << 2) + (a3 << 3) + (s2 << 4) + (s1 << 5) + (s3 << 6); // swapping 1 & 2 is a horizontal mirroring

  int total = min(pattern1, pattern2); // 0 to 127
  if (total < 64) // bit symmetry
    return genome[total];
  else
    return genome[127 - total];
}

// This one is asymmetric in height, which is the z axis
bool Evolver3D::getStatic8(int level, int parentLevel, int x, int y, int z)
{
  int dirX = x % 2 ? 1 : -1;
  int xParent = x >> 1;

  int dirY = y % 2 ? 1 : -1;
  int yParent = y >> 1;

  int dirZ = z % 2 ? 1 : -1;
  int zParent = z >> 1;

  int nearest = (int)get(parentLevel, xParent, yParent, zParent);
  int second = (int)get(parentLevel, xParent + dirX, yParent, zParent) +
    (int)get(parentLevel, xParent, yParent + dirY, zParent) +
    (int)get(parentLevel, xParent, yParent, zParent + dirZ);
  int third = (int)get(parentLevel, xParent + dirX, yParent + dirY, zParent) +
    (int)get(parentLevel, xParent + dirX, yParent, zParent + dirZ) +
    (int)get(parentLevel, xParent, yParent + dirY, zParent + dirZ);
  int fourth = (int)get(parentLevel, xParent + dirX, yParent + dirY, zParent + dirZ);
  int numParents = nearest + second + third + fourth;
  // Note- these two are mandatory
  if (numParents <= 2)
    return false;
  else if (numParents >= 6)
    return true;

  int total = nearest * 32 + second * 8 + third * 4 + fourth; // total = 63
  int up = yUp ? y + yUp : z;
  if (total < 32)
    return genome[total + (up & 1) * 32];
  else
    return !genome[(63 - total) + (up & 1) * 32];
}

// This one is asymmetric in height, which is the z axis
bool Evolver3D::getStatic9(int level, int parentLevel, int x, int y, int z)
{
  int dirX = x % 2 ? 1 : -1;
  int xParent = x >> 1;

  int dirY = y % 2 ? 1 : -1;
  int yParent = y >> 1;

  int dirZ = z % 2 ? 1 : -1;
  int zParent = z >> 1;

  int nearest = (int)get(parentLevel, xParent, yParent, zParent);
  int second = yUp ? (int)get(parentLevel, xParent + dirX, yParent, zParent) +
    (int)get(parentLevel, xParent, yParent, zParent + dirZ)
    : (int)get(parentLevel, xParent + dirX, yParent, zParent) +
    (int)get(parentLevel, xParent, yParent + dirY, zParent);
  int third = (int)get(parentLevel, xParent + dirX, yParent + dirY, zParent) +
    (int)get(parentLevel, xParent + dirX, yParent, zParent + dirZ) +
    (int)get(parentLevel, xParent, yParent + dirY, zParent + dirZ);
  int fourth = (int)get(parentLevel, xParent + dirX, yParent + dirY, zParent + dirZ);
  int v1 = (int)get(parentLevel, xParent - dirX, yParent, zParent);
  int v2 = (int)get(parentLevel, xParent, yParent - dirY, zParent);
  int v3 = (int)get(parentLevel, xParent, yParent, zParent - dirZ);
  int seventh = yUp ? v1 + v3 : v1 + v2;
  int numParents = nearest + second + third + fourth + seventh;
  // Note- these two are mandatory
  if (numParents <= 3)
    return false;
  else if (numParents >= 6)
    return true;

  int total = seventh * 64 + nearest * 32 + second * 8 + third * 4 + fourth; // total = 319
  int up = yUp ? y + yUp : z;
  if (total < 160)
    return genome[total + (up & 1) * 160];
  else
    return !genome[(319 - total) + (up & 1) * 160];
}
