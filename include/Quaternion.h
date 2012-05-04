/**
 * Copyright (c) 2012 Daniel Wiberg
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include "MathUtils.h"
#include "Vector3.h"
#include "Matrix4.h"

#include <stddef.h>
#include <stdint.h>

namespace three {
  
  class Quaternion
  {
    public:
      Quaternion()
        : x(0.0f),
          y(0.0f),
          z(0.0f),
          w(1.0f)
      { }
      
      Quaternion(float _x, float _y, float _z, float _w)
        : x(_x),
          y(_y),
          z(_z),
          w(_w)
      { }
      
      Quaternion(Quaternion const& copy)
        : x(copy.x),
          y(copy.y),
          z(copy.z),
          w(copy.w)
      { }
      
      /// Construct a quaternion from an axis and a rotation around that axis.
      Quaternion(Vector3 const& axis, float angle)
      {
        angle *= 0.5f;
        Vector3 norm = axis.normalized();
        
        float sinAngle = std::sin(angle);
        
        x = norm.x * sinAngle;
        y = norm.y * sinAngle;
        z = norm.z * sinAngle;
        w = std::cos(angle);
      }
      
      Quaternion const& operator = (Quaternion const& copy)
      {
        x = copy.x;
        y = copy.y;
        z = copy.z;
        w = copy.w;
        return *this;
      }
      
      Quaternion operator * (Quaternion const& quat) const
      {
        return Quaternion(w * quat.x + x * quat.w + y * quat.z - z * quat.y,
                          w * quat.y + y * quat.w + z * quat.x - x * quat.z,
                          w * quat.z + z * quat.w + x * quat.y - y * quat.x,
                          w * quat.w - x * quat.x - y * quat.y - z * quat.z);
      }
      
      /// Apply the orientation of this quaternion to a vector.
      Vector3 operator * (Vector3 const& vec) const
      {
        Vector3 norm = vec.normalized();
        
        Quaternion vecQuat(norm.x, norm.y, norm.z, 0.0f);
        Quaternion result = vecQuat * conjugate();
        result = *this * result;
        
        return Vector3(result.x, result.y, result.z);
      }
      
      Quaternion const& normalize()
      {
        float mag = x * x + y * y + z * z + w * w;
        if (std::abs(mag) > EPSILON && std::abs(mag - 1.0f) > EPSILON)
        {
          mag = 1.0f / sqrt(mag);
          x *= mag;
          y *= mag;
          z *= mag;
          w *= mag;
        }
        
        return *this;
      }
      
      Quaternion conjugate() const
      {
        return Quaternion(-x, -y, -z, w);
      }
      
      Matrix4 toMatrix() const;
      
    public:
      float x;
      float y;
      float z;
      float w;
  };
  
}

