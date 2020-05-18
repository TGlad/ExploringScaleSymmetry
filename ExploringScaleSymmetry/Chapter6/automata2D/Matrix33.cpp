// Thomas Lowe, 2020.
// Simple 3x3 matrix class (written prior to uptake of Eigen)
#include "basics.h"

void Matrix33::setIdentity()
{
  row[0].set(1, 0, 0);
  row[1].set(0, 1, 0);
  row[2].set(0, 0, 1);
}

void Matrix33::fromForwardAlignedByUp(const Vector3& forwards, const Vector3& up)
{
  row[2] = forwards;
  row[0] = Vector3::cross(up, forwards);
  row[0].normalise();
  row[1] = Vector3::cross(row[2], row[0]);
}

Matrix33 Matrix33::transposed() const
{
  // simply transpose since we're assuming it is orthonormalised
  Matrix33 result;
  result.row[0].set(row[0].x, row[1].x, row[2].x);
  result.row[1].set(row[0].y, row[1].y, row[2].y);
  result.row[2].set(row[0].z, row[1].z, row[2].z);
  return result;
}

bool Matrix33::invert() 
{
  float determinant;

  float d00 = (row[1].y * row[2].z - row[2].y * row[1].z);
  float d01 = (row[2].y * row[0].z - row[0].y * row[2].z);
  float d02 = (row[0].y * row[1].z - row[1].y * row[0].z);

  // early-out if the matrix is singular
  determinant = (row[0].x * d00) + (row[1].x * d01) + (row[2].x * d02);
  if (determinant == 0.0f)
    return false;

  float d10 = (row[1].z * row[2].x - row[1].x * row[2].z);
  float d11 = (row[0].x * row[2].z - row[0].z * row[2].x);
  float d12 = (row[0].z * row[1].x - row[0].x * row[1].z);

  float d20 = (row[1].x * row[2].y - row[1].y * row[2].x);
  float d21 = (row[0].y * row[2].x - row[0].x * row[2].y);
  float d22 = (row[0].x * row[1].y - row[0].y * row[1].x);

  determinant = 1.0f / determinant;

  row[0].set(d00 * determinant, d01 * determinant, d02 * determinant);
  row[1].set(d10 * determinant, d11 * determinant, d12 * determinant);
  row[2].set(d20 * determinant, d21 * determinant, d22 * determinant);
  return true;
}
Vector3 Matrix33::rotateVector(const Vector3& pos) const
{
  Vector3 result;
  result.x = pos.x * row[0].x + pos.y * row[1].x + pos.z * row[2].x;
  result.y = pos.x * row[0].y + pos.y * row[1].y + pos.z * row[2].y;
  result.z = pos.x * row[0].z + pos.y * row[1].z + pos.z * row[2].z;
  return result;
}
Vector3 Matrix33::inverseRotateVector(const Vector3& pos) const
{
  Vector3 result;
  result.x = pos.x * row[0].x + pos.y * row[0].y + pos.z * row[0].z;
  result.y = pos.x * row[1].x + pos.y * row[1].y + pos.z * row[1].z;
  result.z = pos.x * row[2].x + pos.y * row[2].y + pos.z * row[2].z;
  return result;
}

Matrix33 Matrix33::operator*(const Matrix33& m) const
{
  float xAxis_x = row[0].x * m.row[0].x + row[0].y * m.row[1].x + row[0].z * m.row[2].x;
  float xAxis_y = row[0].x * m.row[0].y + row[0].y * m.row[1].y + row[0].z * m.row[2].y;
  float xAxis_z = row[0].x * m.row[0].z + row[0].y * m.row[1].z + row[0].z * m.row[2].z;

  float yAxis_x = row[1].x * m.row[0].x + row[1].y * m.row[1].x + row[1].z * m.row[2].x;
  float yAxis_y = row[1].x * m.row[0].y + row[1].y * m.row[1].y + row[1].z * m.row[2].y;
  float yAxis_z = row[1].x * m.row[0].z + row[1].y * m.row[1].z + row[1].z * m.row[2].z;

  float zAxis_x = row[2].x * m.row[0].x + row[2].y * m.row[1].x + row[2].z * m.row[2].x;
  float zAxis_y = row[2].x * m.row[0].y + row[2].y * m.row[1].y + row[2].z * m.row[2].y;
  float zAxis_z = row[2].x * m.row[0].z + row[2].y * m.row[1].z + row[2].z * m.row[2].z;

  Matrix33 result;
  result.row[0].set(xAxis_x, xAxis_y, xAxis_z);
  result.row[1].set(yAxis_x, yAxis_y, yAxis_z);
  result.row[2].set(zAxis_x, zAxis_y, zAxis_z);
  return result;
}

void Matrix33::operator*=(const Matrix33& m) 
{
  float xAxis_x = row[0].x * m.row[0].x + row[0].y * m.row[1].x + row[0].z * m.row[2].x;
  float xAxis_y = row[0].x * m.row[0].y + row[0].y * m.row[1].y + row[0].z * m.row[2].y;
  float xAxis_z = row[0].x * m.row[0].z + row[0].y * m.row[1].z + row[0].z * m.row[2].z;

  float yAxis_x = row[1].x * m.row[0].x + row[1].y * m.row[1].x + row[1].z * m.row[2].x;
  float yAxis_y = row[1].x * m.row[0].y + row[1].y * m.row[1].y + row[1].z * m.row[2].y;
  float yAxis_z = row[1].x * m.row[0].z + row[1].y * m.row[1].z + row[1].z * m.row[2].z;

  float zAxis_x = row[2].x * m.row[0].x + row[2].y * m.row[1].x + row[2].z * m.row[2].x;
  float zAxis_y = row[2].x * m.row[0].y + row[2].y * m.row[1].y + row[2].z * m.row[2].y;
  float zAxis_z = row[2].x * m.row[0].z + row[2].y * m.row[1].z + row[2].z * m.row[2].z;

  row[0].set(xAxis_x, xAxis_y, xAxis_z);
  row[1].set(yAxis_x, yAxis_y, yAxis_z);
  row[2].set(zAxis_x, zAxis_y, zAxis_z);
}
Matrix33 Matrix33::operator ~() const
{
  Matrix33 result;
  result.row[0].set(row[0].x, row[1].x, row[2].x);
  result.row[1].set(row[0].y, row[1].y, row[2].y);
  result.row[2].set(row[0].z, row[1].z, row[2].z);
  return result;
}

struct Quat
{
  float x, y, z, w;
};

