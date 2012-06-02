
#include "GLRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "RenderPlugin.h"
#include "Material.h"
#include "Mesh.h"
#include "Geometry.h"
#include "GLObject.h"
#include "GLGeometry.h"

#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <algorithm>

#include <GL/glew.h>

namespace three {

  static inline bool painterSort(RenderObject * obj1, RenderObject * obj2)
  {
    return static_cast<GLObject *>(obj1)->z > static_cast<GLObject *>(obj2)->z;
  }

  GLRenderer::GLRenderer(int windowWidth, int windowHeight, bool fullscreen)
    : Renderer(),
      oldDepthTest(true),
      oldDepthWrite(true),
      oldBlending(NormalBlending)
  {
    setDefaultGLState();
    setViewport(0, 0, windowWidth, windowHeight);

    resetCache();
  }

  GLRenderer::~GLRenderer()
  {
  }

  void GLRenderer::setSize(int width, int height)
  {
    setViewport(0, 0, width, height);
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

    camera->matrixWorldInverse = camera->matrixWorld.inverse();
    projScreenMatrix = camera->projectionMatrix * camera->matrixWorldInverse;

    if (autoUpdateObjects)
      updateGLObjects(scene);

    renderPlugins(renderPluginsPost, scene, camera);

    setRenderTarget(renderTarget);

    if (autoClear || forceClear)
      clear(autoClearColor, autoClearDepth, autoClearStencil);

    // Setup matrices for regular objects
    for (std::vector<RenderObject *>::iterator it = scene->__renderObjects.begin(), end = scene->__renderObjects.end(); it != end; ++it)
    {
      GLObject * glObject = static_cast<GLObject *>(*it);
      Object * object = glObject->sourceObject;

      setupMatrices(object, camera);

      glObject->render = false;

      if (object->visible)
      {
        glObject->render = true;

        if (sortObjects)
        {
          Vector3 position = projScreenMatrix * object->matrixWorld.getPosition();
          glObject->z = position.z;
        }
      }
    }

    // Sort objects according to depth
    std::sort(scene->__renderObjects.begin(), scene->__renderObjects.end(), painterSort);

    // Draw all objects
    if (overrideMaterial)
    {
      setBlending(overrideMaterial->blending);
      setDepthTest(overrideMaterial->depthTest);
      setDepthWrite(overrideMaterial->depthWrite);

      renderObjects(scene->__renderObjects, false, "", camera, /* lights, fog, */ true, overrideMaterial);
    }
    else
    {
      // opaque pass (front-to-back order)
      setBlending(NormalBlending);

      renderObjects(scene->__renderObjects, true, "opaque", camera, /* lights, fog, */ false, 0);

      // transparent pass (back-to-front order)
      renderObjects(scene->__renderObjects, false, "transparent", camera, /* lights, fog, */ true, 0);
    }

    renderPlugins(renderPluginsPost, scene, camera);

    setDepthTest(true);
    setDepthWrite(true);
  }

  void GLRenderer::renderObjects(std::vector<RenderObject *> const& renderList, bool reverse, std::string materialType, Camera * camera, /* lights, fog, */ bool useBlending, Material * overrideMaterial)
  {
    if (reverse)
    {
      for (std::vector<RenderObject *>::const_reverse_iterator it = renderList.rbegin(), end = renderList.rend(); it != end; ++it)
      {
      }
    }
    else
    {
      for (std::vector<RenderObject *>::const_iterator it = renderList.begin(), end = renderList.end(); it != end; ++it)
      {
      }
    }
  }

  void GLRenderer::renderBuffer(Camera * camera, /* lights, fog, */ Material * material, GLGeometry * geometry, GLObject * object)
  {
    bool updateBuffers = false;
    
    if (updateBuffers)
    {
      glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
      //glVertex
    }
  }

  void GLRenderer::updateGLObjects(Scene * scene)
  {
    for (std::vector<Object *>::iterator it = scene->objectsAdded.begin(), end = scene->objectsAdded.end(); it != end; ++it)
      addObject(*it, scene);

    for (std::vector<Object *>::iterator it = scene->objectsRemoved.begin(), end = scene->objectsRemoved.end(); it != end; ++it)
      removeObject(*it, scene);

    scene->objectsAdded.clear();
    scene->objectsRemoved.clear();

    for (std::vector<RenderObject *>::iterator it = scene->__renderObjects.begin(), end = scene->__renderObjects.end(); it != end; ++it)
      updateObject((*it)->sourceObject);

  }

  void GLRenderer::addObject(Object * object, Scene * scene)
  {
    if (object->type() == Mesh::Type)
    {
      Mesh * mesh = dynamic_cast<Mesh *>(object);

      if (!object->__renderObject)
      {
        GLObject * glObject = new GLObject(object);
        scene->__renderObjects.push_back(glObject);
      }

      assert(mesh->geometry);

      createMeshBuffers(mesh->geometry);
    }
  }

  void GLRenderer::removeObject(Object * object, Scene * scene)
  {
  }

  void GLRenderer::updateObject(Object * object)
  {
    if (object->type() == Mesh::Type)
    {
      Mesh * mesh = dynamic_cast<Mesh *>(object);

      Geometry * geom = mesh->geometry;
      GLGeometry * glGeom = static_cast<GLGeometry *>(geom->__renderGeometry);

      if (geom->verticesNeedUpdate)
      {
        glBindBuffer(GL_ARRAY_BUFFER, glGeom->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, geom->vertices.size() * sizeof(Vector3), &geom->vertices[0], geom->dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        geom->verticesNeedUpdate = false;
      }

      if (geom->normalsNeedUpdate)
      {
        glBindBuffer(GL_ARRAY_BUFFER, glGeom->normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, geom->normals.size() * sizeof(Vector3), &geom->normals[0], geom->dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        geom->normalsNeedUpdate = false;
      }

      if (geom->elementsNeedUpdate)
      {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glGeom->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, geom->faces.size() * sizeof(Face), &geom->faces[0], geom->dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        geom->elementsNeedUpdate = false;
      }
    }
  }

  void GLRenderer::createMeshBuffers(Geometry * geometry)
  {
    if (geometry->__renderGeometry)
      return;

    GLGeometry * geom = new GLGeometry(geometry);

    glGenBuffers(1, &geom->vertexBuffer);
    glGenBuffers(1, &geom->normalBuffer);
    glGenBuffers(1, &geom->indexBuffer);

    geometry->verticesNeedUpdate = true;
    geometry->normalsNeedUpdate = true;
    geometry->elementsNeedUpdate = true;
  }

  void GLRenderer::setupMatrices(Object * object, Camera * camera)
  {
    GLObject * glObject = static_cast<GLObject *>(object->__renderObject);

    glObject->modelViewMatrix = camera->matrixWorldInverse * object->matrixWorld;
    glObject->normalMatrix = glObject->modelViewMatrix.inverse();
    glObject->normalMatrix.transpose();
  }

}

