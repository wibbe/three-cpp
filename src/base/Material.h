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

#include "base/Defines.h"
#include "base/Vector3.h"
#include "base/Vector4.h"
#include "base/Color.h"
#include <string>

namespace three {

  // Forward declarations
  class RenderMaterial;
  class Renderer;
  class Texture;

  class Material
  {
    public:
      Material();

      virtual uint32_t type() const = 0;

      virtual void apply(Renderer * renderer) = 0;

      virtual std::string vertexShaderCode() const = 0;
      virtual std::string fragmentShaderCode() const = 0;
      virtual const char * textureName(uint32_t slot) const = 0;
      virtual const char * uniformName(uint32_t slot) const = 0;
      virtual uint32_t textureCount() const = 0;
      virtual uint32_t uniformCount() const = 0;

    public:
      std::string name;
      int materialIndex;
      float opacity;
      bool transparent;

      Blending blending;

      bool depthTest;
      bool depthWrite;

      unsigned char alphaTest;

      bool visible;
      bool needsUpdate;

      // Only for internal use by the renderer
      RenderMaterial * __renderMaterial;
  };

}

