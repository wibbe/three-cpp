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

#include "base/MathUtils.h"

namespace three {
  
  class Vector4
  {
    public:
      Vector4()
        : x(0.0f),
          y(0.0f),
          z(0.0f),
          w(0.0f)
      { }
      
      Vector4(float _x, float _y, float _z, float _w)
        : x(_x),
          y(_y),
          z(_z),
          w(_w)
      { } 
      
      Vector4(Vector4 const& copy)
        : x(copy.x),
          y(copy.y),
          z(copy.z),
          w(copy.w)
      { }
      
      inline Vector4 const& operator = (Vector4 const& copy)
      {
        x = copy.x;
        y = copy.y;
        z = copy.z;
        w = copy.w;
        return *this;
      }
      
      inline Vector4 const& operator += (Vector4 const& vec)
      {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        w += vec.w;
        return *this;
      }
      
      inline Vector4 const& operator -= (Vector4 const& vec)
      {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        w -= vec.w;
        return *this;
      }

      inline Vector4 operator + (Vector4 const& vec) const
      {
        return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
      }
      
      inline Vector4 operator - (Vector4 const& vec) const
      {
        return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
      }

      inline Vector4 operator * (float scalar) const
      {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
      }
      
      inline Vector4 operator - () const
      {
        return Vector4(-x, -y, -z, -w);
      }
      
      inline float length() const
      {
        return std::sqrt(x * x + y * y + z * z + w * w);
      }
      
      Vector4 const& normalize()
      {
        float len = 1.0f / length();
        x *= len;
        y *= len;
        z *= len;
        w *= len;
        
        return *this;
      }
      
      Vector4 normalized() const
      {
        float len = 1.0f / length();
        return Vector4(x * len, y * len, z * len, w * len);
      }

      /// Returns the largest of the three elements
      float max() const
      {
        return three::max(x, three::max(y, three::max(z, w)));
      }

      /// Returns the smallest of the three elements
      float min() const
      {
        return three::min(x, three::min(y, three::min(z, w)));
      }
      
    public:
      float x;
      float y;
      float z;
      float w;
  };
  
  inline float dot(Vector4 const& a, Vector4 const b)
  {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
  }
  
}

