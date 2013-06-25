/**
 * Copyright (c) 2013 Daniel Wiberg
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
#include "base/Vector4.h"
#include "base/Matrix4.h"
#include "base/Object.h"

namespace three {

  class Frustum
  {
    public:
      Frustum();
      Frustum(Matrix4 const& modelViewProjection);

      void setFromMatrix(Matrix4 const& modelViewProjection);

      inline bool contains(Object * object)
      {
        const Vector3 pos = object->matrixWorld.getPosition();
        const float radius = object->boundRadius * object->boundRadiusScale;

        int count = 0;
        for (int i = 0; i < 6; ++i)
          count += (dot(planes[i], pos) + planes[i].w) <= radius ? 0 : 1;

        return count == 6;
      }

      Vector4 planes[6];
  };

}