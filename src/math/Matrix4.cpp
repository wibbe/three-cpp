
#include "math/Matrix4.h"

#include <string.h>
#include <stdio.h>

namespace three {

  const float IDENTITY[16] = { 1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1 };

  const int _00 = 0, _01 = 4, _02 =  8, _03 = 12;
  const int _10 = 1, _11 = 5, _12 =  9, _13 = 13;
  const int _20 = 2, _21 = 6, _22 = 10, _23 = 14;
  const int _30 = 3, _31 = 7, _32 = 11, _33 = 15;

  Matrix4::Matrix4()
  {
    identity();
  }

  Matrix4::Matrix4(Matrix4 const& mat)
  {
    memcpy(m, mat.m, sizeof(float) * 16);
  }

  Matrix4::Matrix4(float m00, float m01, float m02, float m03,
                   float m10, float m11, float m12, float m13,
                   float m20, float m21, float m22, float m23,
                   float m30, float m31, float m32, float m33)
  {
    // Column 0
    m[ 0] = m00;
    m[ 1] = m10;
    m[ 2] = m20;
    m[ 3] = m30;

    // Column 1
    m[ 4] = m01;
    m[ 5] = m11;
    m[ 6] = m21;
    m[ 7] = m31;

    // Column 2
    m[ 8] = m02;
    m[ 9] = m12;
    m[10] = m22;
    m[11] = m32;

    // Column 3
    m[12] = m03;
    m[13] = m13;
    m[14] = m23;
    m[15] = m33;
  }

  void Matrix4::set(float m00, float m01, float m02, float m03,
                    float m10, float m11, float m12, float m13,
                    float m20, float m21, float m22, float m23,
                    float m30, float m31, float m32, float m33)
  {
    // Column 0
    m[ 0] = m00;
    m[ 1] = m10;
    m[ 2] = m20;
    m[ 3] = m30;

    // Column 1
    m[ 4] = m01;
    m[ 5] = m11;
    m[ 6] = m21;
    m[ 7] = m31;

    // Column 2
    m[ 8] = m02;
    m[ 9] = m12;
    m[10] = m22;
    m[11] = m32;

    // Column 3
    m[12] = m03;
    m[13] = m13;
    m[14] = m23;
    m[15] = m33;
  }

  void Matrix4::identity()
  {
    memcpy(m, IDENTITY, sizeof(float) * 16);
  }

  void Matrix4::lookAt(Vector3 const& eye, Vector3 const& target, Vector3 const& up)
  {
    Vector3 z = (eye - target).normalized();
    if (equal(z.length(), 0.0f))
      z.z = 1.0f;

    Vector3 x = cross(up, z).normalized();

    if (equal(x.length(), 0.0f))
    {
      z.x += 0.0001f;
      x = cross(up, z).normalized();
    }

    Vector3 y = cross(z, x);

    m[_00] = x.x; m[_10] = y.x; m[_20] = z.x;
    m[_01] = x.y; m[_11] = y.y; m[_21] = z.y;
    m[_02] = x.z; m[_12] = y.z; m[_22] = z.z;
  }

  void Matrix4::setPosition(Vector3 const& pos)
  {
    m[_03] = pos.x;
    m[_13] = pos.y;
    m[_23] = pos.z;
  }

  Vector3 Matrix4::getPosition() const
  {
    return Vector3(m[_03], m[_13], m[_23]);
  }

  Matrix4 & Matrix4::scale(Vector3 const& scale)
  {
    m[_00] *= scale.x; m[_01] *= scale.y; m[_02] *= scale.z;
    m[_10] *= scale.x; m[_11] *= scale.y; m[_12] *= scale.z;
    m[_20] *= scale.x; m[_21] *= scale.y; m[_22] *= scale.z;
    m[_30] *= scale.x; m[_31] *= scale.y; m[_32] *= scale.z;

    return *this;
  }

  void Matrix4::makePerspective(float fov, float aspect, float near, float far)
  {
    float xymax = near * tan(degToRad(fov * 0.5f));
    float ymin = -xymax;
    float xmin = -xymax;

    float width = xymax - xmin;
    float height = xymax - ymin;

    float depth = far - near;
    float q = -(far + near) / depth;
    float qn = -2.0f * (far * near) / depth;

    float w = 2.0f * near / width;
    w = w / aspect;
    float h = 2.0f * near / height;

    m[0]  = w; m[4]  = 0; m[ 8] =  0; m[12] = 0;
    m[1]  = 0; m[5]  = h; m[ 9] =  0; m[13] = 0;
    m[2]  = 0; m[6]  = 0; m[10] =  q; m[14] = qn;
    m[3]  = 0; m[7]  = 0; m[11] = -1; m[15] = 0;
  }


  void Matrix4::setRotationFromEuler(Vector3 const& rotation)
  {
    float a = std::cos(rotation.x), b = std::sin(rotation.x);
    float c = std::cos(rotation.y), d = std::sin(rotation.y);
    float e = std::cos(rotation.z), f = std::sin(rotation.z);

    float ce = c * e, cf = c * f, de = d * e, df = d * f;

    m[0] = ce + df * b;
    m[4] = de * b - cf;
    m[8] = a * d;
    m[1] = a * f;
    m[5] = a * e;
    m[9] = - b;
    m[2] = cf * b - de;
    m[6] = df + ce * b;
    m[10] = a * c;
  }

  Matrix4 Matrix4::inverse() const
  {
    Matrix4 result;

    result.m[_00] = m[_12]*m[_23]*m[_31] - m[_13]*m[_22]*m[_31] + m[_13]*m[_21]*m[_32] - m[_11]*m[_23]*m[_32] - m[_12]*m[_21]*m[_33] + m[_11]*m[_22]*m[_33];
    result.m[_01] = m[_03]*m[_22]*m[_31] - m[_02]*m[_23]*m[_31] - m[_03]*m[_21]*m[_32] + m[_01]*m[_23]*m[_32] + m[_02]*m[_21]*m[_33] - m[_01]*m[_22]*m[_33];
    result.m[_02] = m[_02]*m[_13]*m[_31] - m[_03]*m[_12]*m[_31] + m[_03]*m[_11]*m[_32] - m[_01]*m[_13]*m[_32] - m[_02]*m[_11]*m[_33] + m[_01]*m[_12]*m[_33];
    result.m[_03] = m[_03]*m[_12]*m[_21] - m[_02]*m[_13]*m[_21] - m[_03]*m[_11]*m[_22] + m[_01]*m[_13]*m[_22] + m[_02]*m[_11]*m[_23] - m[_01]*m[_12]*m[_23];
    result.m[_10] = m[_13]*m[_22]*m[_30] - m[_12]*m[_23]*m[_30] - m[_13]*m[_20]*m[_32] + m[_10]*m[_23]*m[_32] + m[_12]*m[_20]*m[_33] - m[_10]*m[_22]*m[_33];
    result.m[_11] = m[_02]*m[_23]*m[_30] - m[_03]*m[_22]*m[_30] + m[_03]*m[_20]*m[_32] - m[_00]*m[_23]*m[_32] - m[_02]*m[_20]*m[_33] + m[_00]*m[_22]*m[_33];
    result.m[_12] = m[_03]*m[_12]*m[_30] - m[_02]*m[_13]*m[_30] - m[_03]*m[_10]*m[_32] + m[_00]*m[_13]*m[_32] + m[_02]*m[_10]*m[_33] - m[_00]*m[_12]*m[_33];
    result.m[_13] = m[_02]*m[_13]*m[_20] - m[_03]*m[_12]*m[_20] + m[_03]*m[_10]*m[_22] - m[_00]*m[_13]*m[_22] - m[_02]*m[_10]*m[_23] + m[_00]*m[_12]*m[_23];
    result.m[_20] = m[_11]*m[_23]*m[_30] - m[_13]*m[_21]*m[_30] + m[_13]*m[_20]*m[_31] - m[_10]*m[_23]*m[_31] - m[_11]*m[_20]*m[_33] + m[_10]*m[_21]*m[_33];
    result.m[_21] = m[_03]*m[_21]*m[_30] - m[_01]*m[_23]*m[_30] - m[_03]*m[_20]*m[_31] + m[_00]*m[_23]*m[_31] + m[_01]*m[_20]*m[_33] - m[_00]*m[_21]*m[_33];
    result.m[_22] = m[_01]*m[_13]*m[_30] - m[_03]*m[_11]*m[_30] + m[_03]*m[_10]*m[_31] - m[_00]*m[_13]*m[_31] - m[_01]*m[_10]*m[_33] + m[_00]*m[_11]*m[_33];
    result.m[_23] = m[_03]*m[_11]*m[_20] - m[_01]*m[_13]*m[_20] - m[_03]*m[_10]*m[_21] + m[_00]*m[_13]*m[_21] + m[_01]*m[_10]*m[_23] - m[_00]*m[_11]*m[_23];
    result.m[_30] = m[_12]*m[_21]*m[_30] - m[_11]*m[_22]*m[_30] - m[_12]*m[_20]*m[_31] + m[_10]*m[_22]*m[_31] + m[_11]*m[_20]*m[_32] - m[_10]*m[_21]*m[_32];
    result.m[_31] = m[_01]*m[_22]*m[_30] - m[_02]*m[_21]*m[_30] + m[_02]*m[_20]*m[_31] - m[_00]*m[_22]*m[_31] - m[_01]*m[_20]*m[_32] + m[_00]*m[_21]*m[_32];
    result.m[_32] = m[_02]*m[_11]*m[_30] - m[_01]*m[_12]*m[_30] - m[_02]*m[_10]*m[_31] + m[_00]*m[_12]*m[_31] + m[_01]*m[_10]*m[_32] - m[_00]*m[_11]*m[_32];
    result.m[_33] = m[_01]*m[_12]*m[_20] - m[_02]*m[_11]*m[_20] + m[_02]*m[_10]*m[_21] - m[_00]*m[_12]*m[_21] - m[_01]*m[_10]*m[_22] + m[_00]*m[_11]*m[_22];

    result *= 1.0f / determinant();
    return result;
  }

  float Matrix4::determinant() const
  {
    float value = m[_03]*m[_12]*m[_21]*m[_30] - m[_02]*m[_13]*m[_21]*m[_30] - m[_03]*m[_11]*m[_22]*m[_30] + m[_01]*m[_13]*m[_22]*m[_30]+
                  m[_02]*m[_11]*m[_23]*m[_30] - m[_01]*m[_12]*m[_23]*m[_30] - m[_03]*m[_12]*m[_20]*m[_31] + m[_02]*m[_13]*m[_20]*m[_31]+
                  m[_03]*m[_10]*m[_22]*m[_31] - m[_00]*m[_13]*m[_22]*m[_31] - m[_02]*m[_10]*m[_23]*m[_31] + m[_00]*m[_12]*m[_23]*m[_31]+
                  m[_03]*m[_11]*m[_20]*m[_32] - m[_01]*m[_13]*m[_20]*m[_32] - m[_03]*m[_10]*m[_21]*m[_32] + m[_00]*m[_13]*m[_21]*m[_32]+
                  m[_01]*m[_10]*m[_23]*m[_32] - m[_00]*m[_11]*m[_23]*m[_32] - m[_02]*m[_11]*m[_20]*m[_33] + m[_01]*m[_12]*m[_20]*m[_33]+
                  m[_02]*m[_10]*m[_21]*m[_33] - m[_00]*m[_12]*m[_21]*m[_33] - m[_01]*m[_10]*m[_22]*m[_33] + m[_00]*m[_11]*m[_22]*m[_33];
    return value;
  }

  Matrix4 & Matrix4::transpose()
  {
    float tmp;

    tmp = m[1]; m[1] = m[4]; m[4] = tmp;
		tmp = m[2]; m[2] = m[8]; m[8] = tmp;
		tmp = m[6]; m[6] = m[9]; m[9] = tmp;

		tmp = m[3]; m[3] = m[12]; m[12] = tmp;
		tmp = m[7]; m[7] = m[13]; m[13] = tmp;
		tmp = m[11]; m[11] = m[14]; m[14] = tmp;

		return *this;
  }

  Matrix4 Matrix4::operator * (Matrix4 const& mat) const
  {
    Matrix4 result;

    for (int row = 0; row < 4; ++row)
      for (int col = 0; col < 4; ++col)
      {
        const int index = (row * 4) + col;
        //const int index = (col * 4) + row;
        result.m[index] = 0.0f;

        for (int i = 0; i < 4; ++i)
          result.m[index] += m[(i * 4) + col] * mat[(row * 4) + i];
      }

    return result;
  }

  Vector3 Matrix4::operator * (Vector3 const& vector) const
  {
    Vector3 result;

    float d = 1.0f / (m[3] * vector.x + m[7] * vector.y + m[11] * vector.z + m[15]);

		result.x = (m[0] * vector.x + m[4] * vector.y + m[8] * vector.z + m[12]) * d;
		result.y = (m[1] * vector.x + m[5] * vector.y + m[9] * vector.z + m[13]) * d;
		result.z = (m[2] * vector.x + m[6] * vector.y + m[10] * vector.z + m[14]) * d;

		return result;
  }

  Matrix4 & Matrix4::operator *= (float scalar)
  {
    for (int i = 0; i < 16; ++i)
      m[i] *= scalar;

    return *this;
  }

  Matrix4 & Matrix4::operator =  (Matrix4 const& copy)
  {
    memcpy(m, copy.m, sizeof(float) * 16);
    return *this;
  }

  std::string Matrix4::str() const
  {
    char str[1024];
    snprintf(str, 1024, "[ %3.1f, %3.1f, %3.1f, %3.1f ]\n"
                        "| %3.1f, %3.1f, %3.1f, %3.1f |\n"
                        "| %3.1f, %3.1f, %3.1f, %3.1f |\n"
                        "[ %3.1f, %3.1f, %3.1f, %3.1f ]\n",
                        m[ 0], m[ 4], m[ 8], m[12],
                        m[ 1], m[ 5], m[ 9], m[13],
                        m[ 2], m[ 6], m[10], m[14],
                        m[ 3], m[ 7], m[11], m[15]);
    return std::string(str);
  }

}
