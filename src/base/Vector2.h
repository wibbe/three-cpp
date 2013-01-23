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
#include <string>

namespace three {

  class Vector2
  {
    public:
      Vector2()
        : x(0.0f),
          y(0.0f)
      { }

      Vector2(float _x, float _y)
        : x(_x),
          y(_y)
      { }

      Vector2(Vector2 const& copy)
        : x(copy.x),
          y(copy.y)
      { }

      inline Vector2 const& operator = (Vector2 const& copy)
      {
        x = copy.x;
        y = copy.y;
        return *this;
      }

      inline Vector2 const& operator += (Vector2 const& vec)
      {
        x += vec.x;
        y += vec.y;
        return *this;
      }

      inline Vector2 const& operator -= (Vector2 const& vec)
      {
        x -= vec.x;
        y -= vec.y;
        return *this;
      }

      inline Vector2 operator + (Vector2 const& vec) const
      {
        return Vector2(x + vec.x, y + vec.y);
      }

      inline Vector2 operator - (Vector2 const& vec) const
      {
        return Vector2(x - vec.x, y - vec.y);
      }

      inline Vector2 operator * (Vector2 const& vec) const
      {
        return Vector2(x * vec.x, y * vec.y);
      }

      inline Vector2 operator * (float scalar) const
      {
        return Vector2(x * scalar, y * scalar);
      }

      inline Vector2 operator - () const
      {
        return Vector2(-x, -y);
      }

      inline float length() const
      {
        return std::sqrt(x * x + y * y);
      }

      inline float lengthSqrt() const
      {
        return x * x + y * y;
      }

      Vector2 const& normalize()
      {
        float len = 1.0f / length();
        x *= len;
        y *= len;

        return *this;
      }

      Vector2 normalized() const
      {
        float len = 1.0f / length();
        return Vector2(x * len, y * len);
      }

      Vector2 perpendicular() const
      {
        return Vector2(y, -x);
      }

      /// Returns the largest of the three elements
      float max() const
      {
        return three::max(x, y);
      }

      /// Returns the smallest of the three elements
      float min() const
      {
        return three::min(x, y);
      }

      std::string str() const;

    public:
      float x;
      float y;
  };

  inline float dot(Vector2 const& a, Vector2 const b)
  {
    return a.x * b.x + a.y * b.y;
  }

}

