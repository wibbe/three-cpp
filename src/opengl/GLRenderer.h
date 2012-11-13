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

#include "base/Renderer.h"
#include "base/RenderObject.h"
#include "base/Matrix4.h"
#include <vector>
#include <string>
#include <map>

namespace three {

  // Forward declarations
  class GLObject;
  class GLGeometry;
  class GLMaterial;

  class GLRenderer : public Renderer
  {
    public:
      GLRenderer(int windowWidth = 1024, int windowHeight = 768, bool fullscreen = false);
      ~GLRenderer();

      void setSize(int width, int height);
      void setViewport(int x, int y, int width, int height);
      void setRenderTarget(RenderTarget * renderTarget);
      void setDepthTest(bool enabled);
      void setDepthWrite(bool enabled);
      void setBlending(Blending blending);
      void setTexture(Texture * texture, int slot);
      void setClearColor(Color const& color);

      void clear(bool color, bool depth, bool stencil = false);
      void render(Scene * scene, Camera * camera, RenderTarget * renderTarget = 0, bool forceClear = false);

    private:
      void setDefaultGLState();
      void resetCache();

      void renderObjects(std::vector<RenderObject *> const& renderList,
                         bool reverse,
                         std::string materialType,
                         Camera * camera,
                         std::vector<Object *> const& lights,
                         /* fog, */
                         bool useBlending,
                         Material * overrideMaterial);

      void renderObject(Camera * camera,
                        std::vector<Object *> const& lights,
                        /* fog, */
                        Material * material,
                        GLGeometry * geometry,
                        GLObject * object,
                        bool useBlending);

      void updateGLObjects(Scene * scene);
      void updateGeometry(Geometry * geometry);
      void updateObject(Object * object);
      void updateMatrices(Object * object, Camera * camera);

      void addObject(Object * object, Scene * scene);
      void removeObject(Object * object, Scene * scene);

      void setMaterial(Camera * camera,
                       std::vector<Object *> const& lights,
                       /* fog, */
                       Material * material,
                       GLObject * object,
                       bool useBlending);

      void createGeometry(Geometry * geometry);
      void createMaterial(Material * material, GLObject * object);

    private:
      int viewportX;
      int viewportY;
      int viewportWidth;
      int viewportHeight;

      std::vector<RenderPlugin *> renderPluginsPre;
      std::vector<RenderPlugin *> renderPluginsPost;

      Matrix4 projScreenMatrix;
      Matrix4 projScreenMatrixPS;

      bool _currentDepthTest;
      bool _currentDepthWrite;

      Blending _currentBlending;

      uint32_t _currentProgram;
      uint32_t _currentVertexBuffer;
      uint32_t _currentNormalBuffer;
      uint32_t _currentColorBuffer;
      uint32_t _currentTexCoord0Buffer;
      uint32_t _currentIndexBuffer;
      uint32_t _currentTexture[MaxTextureCount];

      std::map<uint32_t, GLMaterial *> _cachedMaterials;
  };

}

