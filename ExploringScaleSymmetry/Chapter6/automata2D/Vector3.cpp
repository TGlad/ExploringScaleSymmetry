// Thomas Lowe, 2020.
// simple 3D vector class (written prior to uptake of Eigen)
#include "basics.h"

void Vector3::rotate(const Matrix33& mat)
{
  float newX = x * mat.row[0].x + y * mat.row[1].x + z * mat.row[2].x;
  float newY = x * mat.row[0].y + y * mat.row[1].y + z * mat.row[2].y;
  float newZ = x * mat.row[0].z + y * mat.row[1].z + z * mat.row[2].z;
  x = newX;
  y = newY;
  z = newZ;
}
void Vector3::inverseRotate(const Matrix33& mat)
{
  float newX = x * mat.row[0].x + y * mat.row[0].y + z * mat.row[0].z;
  float newY = x * mat.row[1].x + y * mat.row[1].y + z * mat.row[1].z;
  float newZ = x * mat.row[2].x + y * mat.row[2].y + z * mat.row[2].z;
  x = newX;
  y = newY;
  z = newZ;
}

void Vector3::clamp(float minVal, float maxVal)
{
  x = clamped(x, minVal, maxVal);
  y = clamped(y, minVal, maxVal);
  z = clamped(z, minVal, maxVal);
}

Vector3 operator *(float f, const Vector3& v)
{
  return Vector3(v.x * f, v.y * f, v.z * f);
}
