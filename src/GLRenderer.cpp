
#include "GLRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "RenderPlugin.h"
#include "Mesh.h"
#include "Geometry.h"
#include "GLObject.h"
#include "GLGeometry.h"

#include <stdio.h>
#include <stdlib.h>
#include <cassert>

#include <GL/glew.h>
#include <GL/glfw.h>

namespace three {

  GLRenderer::GLRenderer(int windowWidth, int windowHeight, bool fullscreen)
    : Renderer(),
      oldDepthTest(true),
      oldDepthWrite(true),
      oldBlending(NormalBlending)
  {
    glfwInit();
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);

    glfwOpenWindow(windowWidth, windowHeight, 8, 8, 8, 8, 24, 0, fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW);

    glewInit();

    setDefaultGLState();
    setViewport(0, 0, windowWidth, windowHeight);

    resetCache();
  }

  GLRenderer::~GLRenderer()
  {
    glfwTerminate();
  }

  void GLRenderer::setViewport(int x, int y, int width, int height)
  {
    viewportX = x;
    viewportY = y;
    viewportWidth = width;
    viewportHeight = height;

    glViewport(x, y, width, height);
  }

  void GLRenderer::setDepthTest(bool enabled)
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

  void GLRenderer::setDepthWrite(bool enabled)
  {
    if (oldDepthWrite != enabled)
    {
      glDepthMask(enabled);
      oldDepthWrite = enabled;
    }
  }

  void GLRenderer::setBlending(Blending blending)
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

        case NormalBlending:
          glEnable(GL_BLEND);
          glBlendEquation(GL_FUNC_ADD);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          break;
      }

      oldBlending = blending;
    }
  }

  void GLRenderer::setRenderTarget(RenderTarget * renderTarget)
  {
  }

  void GLRenderer::clear(bool color, bool depth, bool stencil)
  {
    int bits = 0;
    if (color) bits |= GL_COLOR_BUFFER_BIT;
    if (depth) bits |= GL_DEPTH_BUFFER_BIT;
    if (stencil) bits |= GL_STENCIL_BUFFER_BIT;

    glClear(bits);
  }

  void GLRenderer::setDefaultGLState()
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

  void GLRenderer::resetCache()
  {
    oldDepthTest = true;
    oldDepthWrite = true;
    oldBlending = NormalBlending;
  }
  
  void GLRenderer::render(Scene * scene, Camera * camera, RenderTarget * renderTarget, bool forceClear)
  {
    assert(scene && camera);

    if (autoUpdateScene)
      scene->updateWorldMatrix();

    if (autoUpdateObjects)
      updateGLObjects(scene);

    renderPlugins(renderPluginsPost, scene, camera);

    setRenderTarget(renderTarget);

    if (autoClear || forceClear)
      clear(autoClearColor, autoClearDepth, autoClearStencil);

    renderPlugins(renderPluginsPost, scene, camera);

    setDepthTest(true);
    setDepthWrite(true);
  }

  void GLRenderer::updateGLObjects(Scene * scene)
  {
    for (std::vector<Object *>::iterator it = scene->objectsAdded.begin(), end = scene->objectsAdded.end(); it != end; ++it)
      addObject(*it, scene);

    for (std::vector<Object *>::iterator it = scene->objectsRemoved.begin(), end = scene->objectsRemoved.end(); it != end; ++it)
      removeObject(*it, scene);
  }

  void GLRenderer::addObject(Object * object, Scene * scene)
  {
    if (Mesh * mesh = dynamic_cast<Mesh *>(object))
    {
      if (!object->__renderObject)
        new GLObject(object);

      assert(mesh->geometry);

      createMeshBuffers(mesh->geometry);
    }
  }

  void GLRenderer::removeObject(Object * object, Scene * scene)
  {
  }

  void GLRenderer::updateObject(Object * object)
  {
    if (Mesh * mesh = dynamic_cast<Mesh *>(object))
    {
    }
  }

  void GLRenderer::createMeshBuffers(Geometry * geometry)
  {
    if (geometry->__renderObject)
      return;

    GLGeometry * geom = new GLGeometry(geometry);

    glGenBuffers(1, &geom->vertexBuffer);
    glGenBuffers(1, &geom->normalBuffer);
    glGenBuffers(1, &geom->indexBuffer);

    geometry->verticesNeedUpdate = true;
    geometry->normalsNeedUpdate = true;
    geometry->elementsNeedUpdate = true;
  }

}

