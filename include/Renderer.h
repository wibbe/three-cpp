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
  class Geometry;
  class RenderTarget;
  class RenderPlugin;

  class Renderer
  {
    public:
      Renderer(int windowWidth = 1024, int windowHeight = 768, bool fullscreen = false);
      ~Renderer();

      void setViewport(int x, int y, int width, int height);
      void setRenderTarget(RenderTarget * renderTarget);
      void setDepthTest(bool enabled);
      void setDepthWrite(bool enabled);
      void setBlending(Blending blending);

      void clear(bool color, bool depth, bool stencil = false);

      void render(Scene * scene, Camera * camera, RenderTarget * renderTarget = 0, bool forceClear = false);

      void addPrePlugin(RenderPlugin * plugin);
      void addPostPlugin(RenderPlugin * plugin);

    public:
      bool autoUpdateScene;
      bool autoUpdateObjects;
      bool autoClear;
      bool autoClearColor;
      bool autoClearDepth;
      bool autoClearStencil;

    private:
      void setDefaultGLState();
      void resetCache();

      void renderPlugins(std::vector<RenderPlugin *> & plugins, Scene * scene, Camera * camera);

      void updateGLObjects(Scene * scene);
      void addObject(Object * object, Scene * scene);
      void removeObject(Object * object, Scene * scene);
      void updateObject(Object * object);

      void createMeshBuffers(Geometry * geometry);

    private:
      int viewportX;
      int viewportY;
      int viewportWidth;
      int viewportHeight;

      std::vector<RenderPlugin *> renderPluginsPre;
      std::vector<RenderPlugin *> renderPluginsPost;

      bool oldDepthTest;
      bool oldDepthWrite;

      Blending oldBlending;
  };

}

