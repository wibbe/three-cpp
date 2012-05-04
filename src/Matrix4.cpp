
#include "Matrix4.h"
#include "Math.h"

#include <string.h>

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
  
  void Matrix4::identity()
  {
    memcpy(m, IDENTITY, sizeof(float) * 16);
  }
  
  void Matrix4::setPosition(Vector3 const& pos)
  {
    m[_03] = pos.x;
    m[_13] = pos.y;
    m[_23] = pos.z;
  }
  
  void Matrix4::scale(Vector3 const& scale)
  {
    m[_00] = scale.x;
    m[_11] = scale.y;
    m[_22] = scale.z;
  }
  
  void Matrix4::perspective(float fov, float aspect, float near, float far)
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

    m[0]  = w;
    m[1]  = 0;
    m[2]  = 0;
    m[3]  = 0;

    m[4]  = 0;
    m[5]  = h;
    m[6]  = 0;
    m[7]  = 0;

    m[8]  = 0;
    m[9]  = 0;
    m[10] = q;
    m[11] = -1;

    m[12] = 0;
    m[13] = 0;
    m[14] = qn;
    m[15] = 0;
  }
  
  Matrix4 Matrix4::operator * (Matrix4 const& mat) const
  {
    Matrix4 result;
    
    for (int row = 0; row < 4; ++row)
      for (int col = 0; col < 4; ++col)
      {
        const int index = (row * 4) + col;
        result.m[index] = 0.0f;
        
        for (int i = 0; i < 4; ++i)
          result.m[index] += m[(row * 4) + i] * mat[(i * 4) + col];
      }
    
    return result;
  }
  
}
