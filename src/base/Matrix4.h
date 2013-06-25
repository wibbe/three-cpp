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

#include "base/Vector3.h"
#include <string>

namespace three {

  class Matrix4
  {
    public:
      Matrix4();
      Matrix4(Matrix4 const& mat);
      Matrix4(float m00, float m01, float m02, float m03,
              float m10, float m11, float m12, float m13,
              float m20, float m21, float m22, float m23,
              float m30, float m31, float m32, float m33);

      void set(float m00, float m01, float m02, float m03,
               float m10, float m11, float m12, float m13,
               float m20, float m21, float m22, float m23,
               float m30, float m31, float m32, float m33);

      void identity();

      void lookAt(Vector3 const& eye, Vector3 const& target, Vector3 const& up);

      void setPosition(Vector3 const& pos);
      Vector3 getPosition() const;

      void setRotationFromEuler(Vector3 const& rotation);
      void makePerspective(float fov, float aspect, float near, float far);
      void makeOrthographic(float left, float right, float top, float bottom, float near, float far);

      Matrix4 & scale(Vector3 const& scale);

      Matrix4 inverse() const;
      float determinant() const;

      Matrix4 & transpose();

      Matrix4 operator * (Matrix4 const& mat) const;
      Vector3 operator * (Vector3 const& vector) const;

      Matrix4 & operator *= (float scalar);
      Matrix4 & operator =  (Matrix4 const& copy);

      inline float operator [] (int index) const { return m[index]; }

      float const * data() const { return &m[0]; }

      std::string str() const;

    private:
      // Column major ordering
      // m0 m4 m8  m12
      // m1 m5 m9  m13
      // m2 m6 m10 m14
      // m3 m7 m11 m15
      float m[16];
  };

}

