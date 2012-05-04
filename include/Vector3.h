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

namespace three {

  // Forward declarations
  class Matrix4;
  
  class Vector3
  {
    public:
      Vector3()
        : x(0.0f),
          y(0.0f),
          z(0.0f)
      { }
      
      Vector3(float _x, float _y, float _z)
        : x(_x),
          y(_y),
          z(_z)
      { } 
      
      Vector3(Vector3 const& copy)
        : x(copy.x),
          y(copy.y),
          z(copy.z)
      { }
      
      inline Vector3 const& operator = (Vector3 const& copy)
      {
        x = copy.x;
        y = copy.y;
        z = copy.z;
        return *this;
      }
      
      inline Vector3 const& operator += (Vector3 const& vec)
      {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        return *this;
      }
      
      inline Vector3 const& operator -= (Vector3 const& vec)
      {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        return *this;
      }

      inline Vector3 operator + (Vector3 const& vec) const
      {
        return Vector3(x + vec.x, y + vec.y, z + vec.z);
      }
      
      inline Vector3 operator - (Vector3 const& vec) const
      {
        return Vector3(x - vec.x, y - vec.y, z - vec.z);
      }
      
      inline Vector3 operator - () const
      {
        return Vector3(-x, -y, -z);
      }
      
      inline float length() const
      {
        return std::sqrt(x * x + y * y + z * z);
      }
      
      Vector3 const& normalize()
      {
        float len = 1.0f / length();
        x *= len;
        y *= len;
        z *= len;
        
        return *this;
      }
      
      Vector3 normalized() const
      {
        float len = 1.0f / length();
        return Vector3(x * len, y * len, z * len);
      }

      void getRotationFromMatrix(Matrix4 const& mat, float scale = 1.0f);

      /// Returns the largest of the three elements
      float max() const
      {
        return three::max(x, three::max(y, z));
      }

      /// Returns the smallest of the three elements
      float min() const
      {
        return three::min(x, three::min(y, z));
      }
      
    public:
      float x;
      float y;
      float z;
  };
  
  inline float dot(Vector3 const& a, Vector3 const b)
  {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  inline Vector3 cross(Vector3 const& a, Vector3 const& b)
  {
    return Vector3(a.y * b.z - a.z * b.y,
		               a.z * b.x - a.x * b.z,
		               a.x * b.y - a.y * b.x);
  }
  
}

