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

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Color.h"
#include "base/Face.h"

#include <vector>
#include <stdint.h>

namespace three {

  // Forward declarations
  class RenderGeometry;

  class Geometry
  {
    public:
      typedef std::vector<Vector3> VertexArray;
      typedef std::vector<Color> ColorArray;
      typedef std::vector<Face> FaceArray;
      typedef std::vector<Vector2> TexCoordArray;

    public:
      Geometry();

      void computeBoundingSphere();

    public:
      VertexArray vertices;
      VertexArray normals;
      ColorArray colors;
      TexCoordArray texCoord0;

      FaceArray faces;
      uint32_t faceCount;

      bool hasTangents;
      bool dynamic;

      bool verticesNeedUpdate;
      bool normalsNeedUpdate;
      bool colorsNeedUpdate;
      bool texCoord0NeedUpdate;
      bool elementsNeedUpdate;

      float boundingSphereRadius;

      // Only for internal use by the renderer
      RenderGeometry * __renderGeometry;
  };

}

