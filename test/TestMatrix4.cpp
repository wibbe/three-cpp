
#include "Test.h"

#include "base/Matrix4.h"

using namespace three;

TEST(Matrix4, identity)
{
  Matrix4 mat;

  CHECK_EQ(mat[0], 1.0);
  CHECK_EQ(mat[1], 0.0);
  CHECK_EQ(mat[2], 0.0);
  CHECK_EQ(mat[3], 0.0);
  CHECK_EQ(mat[4], 0.0);
  CHECK_EQ(mat[5], 1.0);
  CHECK_EQ(mat[6], 0.0);
  CHECK_EQ(mat[7], 0.0);
  CHECK_EQ(mat[8], 0.0);
  CHECK_EQ(mat[9], 0.0);
  CHECK_EQ(mat[10], 1.0);
  CHECK_EQ(mat[11], 0.0);
  CHECK_EQ(mat[12], 0.0);
  CHECK_EQ(mat[13], 0.0);
  CHECK_EQ(mat[14], 0.0);
  CHECK_EQ(mat[15], 1.0);
}

TEST(Matrix4, position)
{
  Matrix4 mat;
  mat.setPosition(Vector3(1, 2, 3));

  CHECK_EQ(mat[12], 1.0);
  CHECK_EQ(mat[13], 2.0);
  CHECK_EQ(mat[14], 3.0);
}

TEST(Matrix4, scale)
{
  Matrix4 mat;
  mat.scale(Vector3(1, 2, 3));

  CHECK_EQ(mat[0], 1.0);
  CHECK_EQ(mat[5], 2.0);
  CHECK_EQ(mat[10], 3.0);
}

TEST(Matrix4, lookAt)
{
  Matrix4 mat;
  mat.lookAt(Vector3(0, 0, 0), Vector3(0, 0, -1), Vector3(0, 1, 0));

  // x-axis
  CHECK_EQ(mat[0], 1.0);
  CHECK_EQ(mat[1], 0.0);
  CHECK_EQ(mat[2], 0.0);

  // y-axis
  CHECK_EQ(mat[4], 0.0);
  CHECK_EQ(mat[5], 1.0);
  CHECK_EQ(mat[6], 0.0);

  // z-axis
  CHECK_EQ(mat[8], 0.0);
  CHECK_EQ(mat[9], 0.0);
  CHECK_EQ(mat[10], 1.0);
}