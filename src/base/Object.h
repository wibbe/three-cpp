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

#include "math/Vector3.h"
#include "math/Matrix4.h"

#include <vector>
#include <string>
 #include <stdint.h>

namespace three {

  // Forward declarations
  class RenderObject;

  /**
   * Base class for all objects in Three++
   */
  class Object
  {
    public:
      static uint32_t Type;
    public:
      Object();
      virtual ~Object();

      void add(Object * object);
      void remove(Object * object);

      Object * getChildByName(std::string const& name, bool recursive = false);

      void updateMatrix();
      void updateWorldMatrix(bool force = false);

      virtual uint32_t type() const { return Object::Type; }

    public:
      Object * parent;
      std::vector<Object *> children;

      std::string name;

      float z;

      Vector3 up;

      Vector3 position;
      Vector3 rotation;
      Vector3 scale;

      Matrix4 matrix;
      Matrix4 matrixWorld;

      float boundRadius;
      float boundRadiusScale;

      bool visible;
      bool castShadow;
      bool receiveShadow;

      bool rotationAutoUpdate;

      bool matrixAutoUpdate;
      bool matrixWorldNeedsUpdate;

      // Only for internal use by the renderer
      RenderObject * __renderObject;
  };

}

