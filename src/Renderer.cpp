
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "RenderPlugin.h"

#include <stdio.h>
#include <stdlib.h>
#include <cassert>

#include <GL/glew.h>
#include <GL/glfw.h>

namespace three {

  // There can only exist one renderer at the time.
  static Renderer * g_renderer = 0;

  Renderer::Renderer(int windowWidth, int windowHeight, bool fullscreen)
    : autoUpdateScene(true),
      autoUpdateObjects(true),
      autoClear(true),
      autoClearColor(true),
      autoClearDepth(true),
      autoClearStencil(false),
      oldDepthTest(true),
      oldDepthWrite(true),
      oldBlending(AlphaBlending)
  {
    if (!g_renderer)
    {
      g_renderer = this;

      glfwInit();
      glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
      glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
      glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);

      glfwOpenWindow(windowWidth, windowHeight, 8, 8, 8, 8, 24, 0, fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW);

      glewInit();

      setDefaultGLState();
      setViewport(0, 0, windowWidth, windowHeight);
    }
    else
    {
      fprintf(stderr, "There can only be one allocated renderer at all times.");
      exit(1);
    }
  }

  Renderer::~Renderer()
  {
    glfwTerminate();

    if (g_renderer == this)
      g_renderer = 0;
  }

  void Renderer::setViewport(int x, int y, int width, int height)
  {
    viewportX = x;
    viewportY = y;
    viewportWidth = width;
    viewportHeight = height;

    glViewport(x, y, width, height);
  }

  void Renderer::setDepthTest(bool enabled)
  {
    if (oldDepthTest != enabled)
    {
      if (enabled)
        glEnable(GL_DEPTH_TEST);
      else
        glDisable(GL_DEPTH_TEST);

      oldDepthTest = enabled;
    }
  }

  void Renderer::setDepthWrite(bool enabled)
  {
    if (oldDepthWrite != enabled)
    {
      glDepthMask(enabled);
      oldDepthWrite = enabled;
    }
  }

  void Renderer::setBlending(Blending blending)
  {
    if (oldBlending != blending)
    {
      switch (blending)
      {
        case NoBlending:
          glDisable(GL_BLEND);
          break;

        case AdditiveBlending:
          glEnable(GL_BLEND);
          glBlendEquation(GL_FUNC_ADD);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE);
          break;
      
        case SubstractiveBlending:
          glEnable(GL_BLEND);
          glBlendEquation(GL_FUNC_ADD);
          glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
          break;

        case MultiplyBlending:
          glEnable(GL_BLEND);
          glBlendEquation(GL_FUNC_ADD);
          glBlendFunc(GL_ZERO, GL_SRC_COLOR);
          break;

        case AlphaBlending:
          glEnable(GL_BLEND);
          glBlendEquation(GL_FUNC_ADD);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          break;
      }

      oldBlending = blending;
    }
  }

  void Renderer::setRenderTarget(RenderTarget * renderTarget)
  {
  }

  void Renderer::clear(bool color, bool depth, bool stencil)
  {
    int bits = 0;
    if (color) bits |= GL_COLOR_BUFFER_BIT;
    if (depth) bits |= GL_DEPTH_BUFFER_BIT;
    if (stencil) bits |= GL_STENCIL_BUFFER_BIT;

    glClear(bits);
  }

  void Renderer::setDefaultGLState()
  {
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0);
    glClearStencil(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void Renderer::addPrePlugin(RenderPlugin * plugin)
  {
    plugin->init(this);
    renderPluginsPre.push_back(plugin);
  }

  void Renderer::addPostPlugin(RenderPlugin * plugin)
  {
    plugin->init(this);
    renderPluginsPost.push_back(plugin);
  }
  
  void Renderer::render(Scene * scene, Camera * camera, RenderTarget * renderTarget, bool forceClear)
  {
    assert(scene && camera);

    if (autoUpdateScene)
      scene->updateWorldMatrix();

    setRenderTarget(renderTarget);

    if (autoClear || forceClear)
      clear(autoClearColor, autoClearDepth, autoClearStencil);

    setDepthTest(true);
    setDepthWrite(true);
  }

}
