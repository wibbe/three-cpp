
#include "GLRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "RenderPlugin.h"
#include "Material.h"
#include "Mesh.h"
#include "Geometry.h"
#include "Texture.h"
#include "GLObject.h"
#include "GLGeometry.h"
#include "GLTexture.h"
#include "GLShaders.h"

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

  static inline GLenum wrappingToGL(WrappingMode mode)
  {
    switch (mode)
    {
      case RepeatWrapping:
        return GL_REPEAT;

      case ClampToEdgeWrapping:
        return GL_CLAMP_TO_EDGE;

      case MirroredRepeatWrapping:
        return GL_MIRRORED_REPEAT;
    }

    return 0;
  }

  static inline GLenum filterToGL(Filter filter)
  {
    switch (filter)
    {
      case NearestFilter:
        return GL_NEAREST;
      case NearestMipMapFilter:
        return GL_NEAREST_MIPMAP_NEAREST;
      case NearestMipMapLinearFilter:
        return GL_NEAREST_MIPMAP_LINEAR;
      case LinearFilter:
        return GL_LINEAR;
      case LinearMipMapFilter:
        return GL_LINEAR_MIPMAP_NEAREST;
      case LinearMipMapLinearFilter:
        return GL_LINEAR_MIPMAP_LINEAR;
    }

    return 0;
  }

  static inline GLenum typeToGL(Type type)
  {
    switch (type)
    {
      case ByteType:
        return GL_BYTE;
      case UnsignedByteType:
        return GL_UNSIGNED_BYTE;
      case ShortType:
        return GL_SHORT;
      case UnsignedShortType:
        return GL_UNSIGNED_SHORT;
      case IntType:
        return GL_INT;
      case UnsignedIntType:
        return GL_UNSIGNED_INT;
      case FloatType:
        return GL_FLOAT;
    }
    return 0;
  }

  static inline GLenum formatToGL(Format format)
  {
    switch (format)
    {
      case AlphaFormat:
        return GL_ALPHA;
      case RGBFormat:
        return GL_RGB;
      case RGBAFormat:
        return GL_RGBA;
      case LuminanceFormat:
        return GL_LUMINANCE;
      case LuminanceAlphaFormat:
        return GL_LUMINANCE_ALPHA;
    }

    return 0;
  }

  static inline bool isPowerOfTwo(int value)
  {
    return (value & (value - 1)) == 0;
  }

  GLRenderer::GLRenderer(int windowWidth, int windowHeight, bool fullscreen)
    : Renderer(),
      oldDepthTest(true),
      oldDepthWrite(true),
      oldBlending(NormalBlending)
  {
    if (!GLEW_ARB_framebuffer_object)
    {
      fprintf(stderr, "OpenGL Error: ARB_framebuffer_object not supported\n");
      return;
    }

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

  void GLRenderer::setTexture(Texture * texture, int slot)
  {
    assert(texture);

    GLTexture * glTex = static_cast<GLTexture *>(texture->__renderTexture);

    if (texture->needsUpdate)
    {
      if (!glTex)
      {
        glTex = new GLTexture(texture);
        glGenTextures(1, &glTex->id);
      }

      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, glTex->id);

  	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterToGL(texture->minFilter));
  	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterToGL(texture->magFilter));
  	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingToGL(texture->wrapS));
  	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingToGL(texture->wrapT));

  	  glTexImage2D(GL_TEXTURE_2D, 0, 
  	               formatToGL(texture->format), 
  	               texture->width, texture->height, 0, 
  	               formatToGL(texture->format), 
  	               typeToGL(texture->type), texture->image);

      bool isImagePowerOfTwo = isPowerOfTwo(texture->width) && isPowerOfTwo(texture->height);

      if (texture->generateMipmaps && isImagePowerOfTwo)
        glGenerateMipmap(GL_TEXTURE_2D);

      texture->needsUpdate = false;
    }
    else
    {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, glTex->id);

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

      renderObjects(scene->__renderObjects, false, "", camera, scene->lights, /* fog, */ true, overrideMaterial);
    }
    else
    {
      // opaque pass (front-to-back order)
      setBlending(NormalBlending);

      renderObjects(scene->__renderObjects, true, "opaque", camera, scene->lights, /* fog, */ false, 0);

      // transparent pass (back-to-front order)
      renderObjects(scene->__renderObjects, false, "transparent", camera, scene->lights, /* fog, */ true, 0);
    }

    renderPlugins(renderPluginsPost, scene, camera);

    setDepthTest(true);
    setDepthWrite(true);
  }

  void GLRenderer::renderObjects(std::vector<RenderObject *> const& renderList, 
                                 bool reverse, 
                                 std::string materialType, 
                                 Camera * camera,
                                 std::vector<Object *> const& lights,
                                 /* fog, */ 
                                 bool useBlending, 
                                 Material * overrideMaterial)
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

  void GLRenderer::renderBuffer(Camera * camera, 
                                std::vector<Object *> const& lights, 
                                /* fog, */ 
                                Material * material, 
                                GLGeometry * geometry, 
                                GLObject * object)
  {
    if (!material->visible)
      return;

    setProgram(camera, lights, /* fog, */ material, object);

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

  void GLRenderer::setProgram(Camera * camera,
                              std::vector<Object *> const& lights,
                              /* fog, */
                              Material * material,
                              GLObject * object)
  {
    GLMaterial * glMat = static_cast<GLMaterial *>(material->__renderMaterial);

    if (!glMat || material->needsUpdate)
    {
      glMat = new GLMaterial(material);
      setupMaterial(material, object);
      material->needsUpdate = false;
    }
  }

  void GLRenderer::setupMaterial(Material * material, GLObject * object)
  {
    GLMaterial * glMat = static_cast<GLMaterial *>(material->__renderMaterial);
  }

}

