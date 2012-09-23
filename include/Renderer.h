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

#include "Defines.h"
#include <vector>

namespace three {

  // Forward declarations
  class Object;
  class Mesh;
  class Scene;
  class Camera;
  class Material;
  class Geometry;
  class Texture;
  class Code;
  class RenderTarget;
  class RenderPlugin;

  class Renderer
  {
    public:
      Renderer();
      virtual ~Renderer();

      virtual void setSize(int width, int height) = 0;
      virtual void setViewport(int x, int y, int width, int height) = 0;
      virtual void setRenderTarget(RenderTarget * renderTarget) = 0;
      virtual void setDepthTest(bool enabled) = 0;
      virtual void setDepthWrite(bool enabled) = 0;
      virtual void setBlending(Blending blending) = 0;
      virtual void setTexture(Texture * texture, int slot) = 0;

      virtual void clear(bool color, bool depth, bool stencil = false) = 0;

      virtual void render(Scene * scene, Camera * camera, RenderTarget * renderTarget = 0, bool forceClear = false) = 0;

      void addPrePlugin(RenderPlugin * plugin);
      void addPostPlugin(RenderPlugin * plugin);

    public:
      bool autoUpdateScene;
      bool autoUpdateObjects;
      bool autoClear;
      bool autoClearColor;
      bool autoClearDepth;
      bool autoClearStencil;
      bool sortObjects;

      Material * overrideMaterial;

    protected:
      virtual void resetCache() = 0;
      void renderPlugins(std::vector<RenderPlugin *> & plugins, Scene * scene, Camera * camera);

    protected:
      std::vector<RenderPlugin *> renderPluginsPre;
      std::vector<RenderPlugin *> renderPluginsPost;
  };

  int __dummyCode(int value);
  static void registerCode(Code * code);

}

