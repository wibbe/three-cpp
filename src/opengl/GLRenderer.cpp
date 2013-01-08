
#include "opengl/GLRenderer.h"
#include "opengl/GLRenderTarget.h"

#include "base/Scene.h"
#include "base/Camera.h"
#include "base/RenderPlugin.h"
#include "base/Material.h"
#include "base/Mesh.h"
#include "base/Geometry.h"
#include "base/Texture.h"
#include "base/RenderTarget.h"
#include "base/Code.h"
#include "base/MurmurHash.h"
#include "material/DefaultGLShaders.h"

#include "opengl/GLObject.h"
#include "opengl/GLGeometry.h"
#include "opengl/GLTexture.h"
#include "opengl/GLMaterial.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <cassert>
#include <algorithm>
#include <iostream>

#include <GL/glew.h>

namespace three {

  static const char * glErrorStr(GLenum error)
  {
    switch (error)
    {
      case GL_NO_ERROR:
        return "No error has been recorded";

      case GL_INVALID_ENUM:
        return "An unacceptable value is specified for an enumerated argument.";

      case GL_INVALID_VALUE:
        return "A numeric argument is out of range.";

      case GL_INVALID_OPERATION:
        return "The specified operation is not allowed in the current state.";

      case GL_STACK_OVERFLOW:
        return "This command would cause a stack overflow.";

      case GL_STACK_UNDERFLOW:
        return "This command would cause a stack underflow.";

      case GL_OUT_OF_MEMORY:
        return "There is not enough memory left to execute the command.";

      case GL_TABLE_TOO_LARGE:
        return "The specified table exceeds the implementation’s maximum supported table size.";
    }

    return "Unknown Error";
  }

  #ifdef DEBUG
    #define CHECK_GL() do { if (GLenum code = glGetError() != GL_NO_ERROR) fprintf(stderr, "%s:%d: (%d)%s\n", __FILE__, __LINE__, code, glErrorStr(code)); } while (0)
  #else
    #define CHECK_GL()
  #endif

  static inline bool painterSort(GLObject * obj1, GLObject * obj2)
  {
    return obj1->z > obj2->z;
  }

  enum SHADER_ATTRIB_LOCATION
  {
    POSITION_ATTRIB_LOCATION = 0,
    NORMAL_ATTRIB_LOCATION,
    UV0_ATTRIB_LOCATION,
    UV1_ATTRIB_LOCATION,
    COLOR_ATTRIB_LOCATION,
    TANGENT_ATTRIB_LOCATION
  };

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
      case DepthFormat:
        return GL_DEPTH_COMPONENT;
    }

    return 0;
  }

  static inline bool isPowerOfTwo(int value)
  {
    return (value & (value - 1)) == 0;
  }

  static void showLogInfo(GLuint object,
                          void (*glGet__iv)(GLuint, GLenum, GLint*),
                          void (*glGet__InfoLog)(GLuint, GLsizei, GLsizei*, GLchar*))
  {
    int logLength;
    char * log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &logLength);
    log = new char[logLength];
    glGet__InfoLog(object, logLength, NULL, log);
    fprintf(stderr, "%s", log);

    delete[] log;
  }

  static uint32_t compileShader(std::string const& code, unsigned int type)
  {
    const GLint length = code.size();
    const GLchar * data[1];
    data[0] = code.c_str();

    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar **)data, &length);
    glCompileShader(shader);

    int ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
      fprintf(stderr, "Failed to compile shader:\n");
      showLogInfo(shader, glGetShaderiv, glGetShaderInfoLog);
      fprintf(stderr, "----------------------------\n%s----------------------------", code.c_str());
      glDeleteShader(shader);
      return 0;
    }
    else
    {
      //std::cout << "Compiled shader:" << std::endl << "----------------------------" << std::endl << code << std::endl << "----------------------------" << std::endl;
    }

    CHECK_GL();

    return shader;
  }

  static void setupTexture(Texture * texture, GLTexture * glTex, int slot = 0)
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(glTex->type, glTex->id);

    if (texture->type == Texture2D)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterToGL(texture->minFilter));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterToGL(texture->magFilter));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingToGL(texture->wrapS));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingToGL(texture->wrapT));

      if (!texture->generateMipmaps)
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);

      glTexImage2D(GL_TEXTURE_2D, 0,
                   formatToGL(texture->format),
                   texture->width, texture->height, 0,
                   formatToGL(texture->format),
                   typeToGL(texture->imageDataType), texture->images[0]);
    }
    else // CubeMap
    {
      GLenum format = formatToGL(texture->format);
      GLenum type = typeToGL(texture->imageDataType);

      for (int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                     format, texture->width, texture->height, 0,
                     format, type, texture->images[i]);
    }

    bool isImagePowerOfTwo = isPowerOfTwo(texture->width) && isPowerOfTwo(texture->height);

    if (texture->generateMipmaps && isImagePowerOfTwo)
      glGenerateMipmap(glTex->type);

    texture->needsUpdate = false;
    glBindTexture(glTex->type, 0);
    CHECK_GL();
  }


  GLRenderer::GLRenderer(int windowWidth, int windowHeight, bool fullscreen)
    : Renderer(),
      _currentDepthTest(true),
      _currentDepthWrite(true),
      _currentBlending(NormalBlending),
      _currentFrameBuffer(0),
      _currentProgram(0),
      _currentVertexBuffer(0),
      _currentNormalBuffer(0),
      _currentColorBuffer(0),
      _currentTexCoord0Buffer(0),
      _currentIndexBuffer(0)
  {
    if (!GLEW_ARB_framebuffer_object)
    {
      fprintf(stderr, "OpenGL Error: ARB_framebuffer_object not supported\n");
      return;
    }

    memset(_currentTexture, 0, sizeof(uint32_t) * MaxTextureCount);

    setDefaultGLState();
    setViewport(0, 0, windowWidth, windowHeight);

    resetCache();

    initializeDefaultGLShaders();
  }

  GLRenderer::~GLRenderer()
  {
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
    _currentDepthTest = true;
    _currentDepthWrite = true;
    _currentBlending = NormalBlending;
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
    if (_currentDepthTest != enabled)
    {
      if (enabled)
        glEnable(GL_DEPTH_TEST);
      else
        glDisable(GL_DEPTH_TEST);

      _currentDepthTest = enabled;
    }
  }

  void GLRenderer::setDepthWrite(bool enabled)
  {
    if (_currentDepthWrite != enabled)
    {
      glDepthMask(enabled);
      _currentDepthWrite = enabled;
    }
  }

  void GLRenderer::setBlending(Blending blending)
  {
    if (_currentBlending != blending)
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

      _currentBlending = blending;
    }
  }

  void GLRenderer::setClearColor(Color const& color)
  {
    glClearColor(color.r, color.g, color.b, color.a);
  }

  void GLRenderer::setTexture(Texture * texture, int slot)
  {
    if (!texture)
    {
      if (_currentTexture[slot] != 0)
      {
        _currentTexture[slot] = 0;
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
      }
      return;
    }

    GLTexture * glTex = static_cast<GLTexture *>(texture->__renderTexture);

    if (!glTex || texture->needsUpdate)
    {
      if (!glTex)
      {
        glTex = new GLTexture(texture);
        glTex->type = texture->type == Texture2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
        glGenTextures(1, &glTex->id);
      }

      setupTexture(texture, glTex, slot);
    }

    if (_currentTexture[slot] != glTex->id)
    {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(glTex->type, glTex->id);
      _currentTexture[slot] = glTex->id;
    }
  }

  void GLRenderer::setRenderTarget(RenderTarget * renderTarget)
  {
    if (!renderTarget)
    {
      if (_currentFrameBuffer != 0)
      {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
        _currentFrameBuffer = 0;
      }

      return;
    }

    GLRenderTarget * glTarget = static_cast<GLRenderTarget *>(renderTarget->__renderTarget);

    if (!glTarget)
    {
      GLTexture * colorTex = renderTarget->colorTexture ? static_cast<GLTexture *>(renderTarget->colorTexture->__renderTexture) : 0;
      GLTexture * depthTex = renderTarget->depthTexture ? static_cast<GLTexture *>(renderTarget->depthTexture->__renderTexture) : 0;

      if (renderTarget->colorTexture && !colorTex)
      {
        colorTex = new GLTexture(renderTarget->colorTexture);
        colorTex->type = GL_TEXTURE_2D;
        glGenTextures(1, &colorTex->id);
        setupTexture(renderTarget->colorTexture, colorTex);
      }

      if (renderTarget->depthTexture && !depthTex)
      {
        depthTex = new GLTexture(renderTarget->depthTexture);
        depthTex->type = GL_TEXTURE_2D;
        glGenTextures(1, &depthTex->id);
        setupTexture(renderTarget->depthTexture, depthTex);
      }

      glTarget = new GLRenderTarget(renderTarget);
      glGenFramebuffers(1, &glTarget->id);
      glBindFramebuffer(GL_FRAMEBUFFER, glTarget->id);

      CHECK_GL();

      if (colorTex)
      {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex->id, 0);
      }
      if (depthTex)
      {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex->id, 0);
      }
      else
      {
        uint32_t id;
        glGenRenderbuffers(1, &id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, renderTarget->width, renderTarget->height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id);
      }

      CHECK_GL();

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      {
        fprintf(stderr, "There was a problem costructing the RenderTarget\n");
        glDeleteFramebuffers(1, &glTarget->id);
        glTarget->id = 0;
      }

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      _currentFrameBuffer = 0;
    }

    if (_currentFrameBuffer != glTarget->id)
    {
      _currentFrameBuffer = glTarget->id;
      glBindFramebuffer(GL_FRAMEBUFFER, glTarget->id);

      //uint32_t attachments[1] = { GL_COLOR_ATTACHMENT0 };
      //glDrawBuffers(1, attachments);

      glViewport(0, 0, renderTarget->width, renderTarget->height);

      CHECK_GL();
    }
  }

  void GLRenderer::clear(bool color, bool depth, bool stencil)
  {
    int bits = 0;
    if (color) bits |= GL_COLOR_BUFFER_BIT;
    if (depth) bits |= GL_DEPTH_BUFFER_BIT;
    if (stencil) bits |= GL_STENCIL_BUFFER_BIT;

    glClear(bits);
  }

  void GLRenderer::render(Scene * scene, Camera * camera, RenderTarget * renderTarget, bool forceClear)
  {
    assert(scene && camera);

    if (autoUpdateScene)
      scene->updateWorldMatrix();

    camera->matrixWorldInverse = camera->matrixWorld.inverse();
    camera->positionWorld = camera->matrixWorld * camera->position;
    projScreenMatrix = camera->matrixWorldInverse * camera->projectionMatrix;

    if (autoUpdateObjects)
      updateGLObjects(scene);

    renderPlugins(renderPluginsPre, scene, camera);

    setRenderTarget(renderTarget);

    if (autoClear || forceClear)
      clear(autoClearColor, autoClearDepth, autoClearStencil);

    _renderObjects.clear();
    _renderObject.reserver(scene->renderObjects.size());

    // Setup matrices for regular objects
    for (std::vector<BackendObject *>::iterator it = scene->__renderObjects.begin(), end = scene->__renderObjects.end(); it != end; ++it)
    {
      GLObject * glObject = static_cast<GLObject *>(*it);
      Object * object = glObject->sourceObject;

      if (object->visible)
      {
        updateMatrices(object, camera);
        _renderObjects.push_back(glObject);

        if (sortObjects)
        {
          Vector3 position = projScreenMatrix * object->matrixWorld.getPosition();
          glObject->z = position.z;
        }
      }
    }

    // Sort objects according to depth
    if (sortObjects)
      std::sort(_renderObjects.begin(), _renderObjects.end(), painterSort);

    // Draw all objects
    if (overrideMaterial)
    {
      setBlending(overrideMaterial->blending);
      setDepthTest(overrideMaterial->depthTest);
      setDepthWrite(overrideMaterial->depthWrite);

      renderObjects(_renderObjects, false, "", camera, scene->lights, /* fog, */ true, overrideMaterial);
    }
    else
    {
      // opaque pass (front-to-back order)
      setBlending(NormalBlending);
      renderObjects(_renderObjects, false, "opaque", camera, scene->lights, /* fog, */ false, 0);

      // transparent pass (back-to-front order)
      renderObjects(_renderObjects, true, "transparent", camera, scene->lights, /* fog, */ true, 0);
    }

    renderPlugins(renderPluginsPost, scene, camera);

    setDepthTest(true);
    setDepthWrite(true);
  }

  void GLRenderer::renderObjects(std::vector<BackendObject *> const& renderList,
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
      for (std::vector<BackendObject *>::const_reverse_iterator it = renderList.rbegin(), end = renderList.rend(); it != end; ++it)
      {
        GLObject * object = static_cast<GLObject *>(*it);
        Material * material = overrideMaterial ? overrideMaterial : *object->material;

        if (object->render && material->transparent)
          renderObject(camera, lights, material, object->geometry, object, useBlending);
      }
    }
    else
    {
      for (std::vector<BackendObject *>::const_iterator it = renderList.begin(), end = renderList.end(); it != end; ++it)
      {
        GLObject * object = static_cast<GLObject *>(*it);
        Material * material = overrideMaterial ? overrideMaterial : *object->material;

        if (object->render && !material->transparent)
          renderObject(camera, lights, material, object->geometry, object, useBlending);
      }
    }
  }

  void GLRenderer::renderObject(Camera * camera,
                                std::vector<Object *> const& lights,
                                /* fog, */
                                Material * material,
                                GLGeometry * geometry,
                                GLObject * object,
                                bool useBlending)
  {
    if (!material->visible)
      return;

    setMaterial(camera, lights, /* fog, */ material, object, useBlending);

    // Bind buffers
    if (_currentVertexBuffer != geometry->vertexBuffer)
    {
      if (geometry->vertexBuffer)
      {
        glEnableVertexAttribArray(POSITION_ATTRIB_LOCATION);
        glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
        glVertexAttribPointer(POSITION_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
      }
      else
        glDisableVertexAttribArray(POSITION_ATTRIB_LOCATION);

      _currentVertexBuffer = geometry->vertexBuffer;
    }

    if (_currentNormalBuffer != geometry->normalBuffer)
    {
      if (geometry->normalBuffer)
      {
        glEnableVertexAttribArray(NORMAL_ATTRIB_LOCATION);
        glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
        glVertexAttribPointer(NORMAL_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
      }
      else
        glDisableVertexAttribArray(NORMAL_ATTRIB_LOCATION);

      _currentNormalBuffer = geometry->normalBuffer;
    }

    if (_currentColorBuffer != geometry->colorBuffer)
    {
      if (geometry->colorBuffer)
      {
        glEnableVertexAttribArray(COLOR_ATTRIB_LOCATION);
        glBindBuffer(GL_ARRAY_BUFFER, geometry->colorBuffer);
        glVertexAttribPointer(COLOR_ATTRIB_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
      }
      else
        glDisableVertexAttribArray(COLOR_ATTRIB_LOCATION);

      _currentColorBuffer = geometry->colorBuffer;
    }

    if (_currentTexCoord0Buffer != geometry->texCoord0Buffer)
    {
      if (geometry->texCoord0Buffer)
      {
        glEnableVertexAttribArray(UV0_ATTRIB_LOCATION);
        glBindBuffer(GL_ARRAY_BUFFER, geometry->texCoord0Buffer);
        glVertexAttribPointer(UV0_ATTRIB_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
      }
      else
        glDisableVertexAttribArray(UV0_ATTRIB_LOCATION);

      _currentTexCoord0Buffer = geometry->texCoord0Buffer;
    }

    if (_currentIndexBuffer != geometry->indexBuffer)
    {
      _currentIndexBuffer = geometry->indexBuffer;
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _currentIndexBuffer);
    }

    // Render buffer
    if (_currentIndexBuffer)
      glDrawElements(GL_TRIANGLES, geometry->faceCount, GL_UNSIGNED_SHORT, (void *)0);
    else
      glDrawArrays(GL_TRIANGLES, 0, geometry->faceCount);
  }

  void GLRenderer::setMaterial(Camera * camera,
                               std::vector<Object *> const& lights,
                               /* fog, */
                               Material * material,
                               GLObject * object,
                               bool useBlending)
  {
    GLMaterial * glMat = static_cast<GLMaterial *>(material->__renderMaterial);

    if (!glMat)
    {
      if (!glMat)
      {
        const std::string vertexShaderCode = material->vertexShaderCode();
        const std::string fragmentShaderCode = material->fragmentShaderCode();

        const uint32_t vertexShaderHash = murmurHash(vertexShaderCode.c_str(), vertexShaderCode.size());
        const uint32_t materialHash = murmurHash(fragmentShaderCode.c_str(), fragmentShaderCode.size(), vertexShaderHash);

        std::map<uint32_t, GLMaterial *>::iterator result = _cachedMaterials.find(materialHash);
        if (result == _cachedMaterials.end())
        {
          glMat = new GLMaterial(material->uniformCount());
          material->__renderMaterial = glMat;
          createMaterial(material, object);

          _cachedMaterials.insert(std::make_pair(materialHash, glMat));
        }
        else
        {
          glMat = result->second;
          material->__renderMaterial = glMat;
        }
      }
    }

    if (glMat->program)
    {
      if (glMat->program != _currentProgram)
      {
        _currentProgram = glMat->program;
        glUseProgram(_currentProgram);
      }

      // Set default uniforms
      glUniformMatrix4fv(glMat->objectMatrix, 1, 0, object->sourceObject->matrixWorld.data());
      glUniformMatrix4fv(glMat->modelViewMatrix, 1, 0, object->modelViewMatrix.data());
      glUniformMatrix4fv(glMat->projectionMatrix, 1, 0, camera->projectionMatrix.data());
      //glMat->viewMatrix
      glUniformMatrix4fv(glMat->normalMatrix, 1, 0, object->normalMatrix.data());

      {
        Vector3 pos = camera->matrixWorld.getPosition();
        glUniform3f(glMat->cameraPosition, pos.x, pos.y, pos.z);
      }

      // Set material specific uniforms and textures
      material->apply(this);
    }

    if (useBlending)
      setBlending(material->blending);

    setDepthTest(material->depthTest);
    setDepthWrite(material->depthWrite);
  }

  void GLRenderer::updateGLObjects(Scene * scene)
  {
    for (std::vector<Object *>::iterator it = scene->objectsAdded.begin(), end = scene->objectsAdded.end(); it != end; ++it)
      addObject(*it, scene);

    for (std::vector<Object *>::iterator it = scene->objectsRemoved.begin(), end = scene->objectsRemoved.end(); it != end; ++it)
      removeObject(*it, scene);

    scene->objectsAdded.clear();
    scene->objectsRemoved.clear();

    for (std::vector<BackendObject *>::iterator it = scene->__renderObjects.begin(), end = scene->__renderObjects.end(); it != end; ++it)
      updateObject((*it)->sourceObject);
  }

  void GLRenderer::addObject(Object * object, Scene * scene)
  {
    if (object->type() == Mesh::Type)
    {
      if (!object->__renderObject)
      {
        GLObject * glObject = new GLObject(object);
        scene->__renderObjects.push_back(glObject);
      }
    }
  }

  void GLRenderer::removeObject(Object * object, Scene * scene)
  {
    scene->__renderObjects.erase(std::remove(scene->__renderObjects.begin(),
                                             scene->__renderObjects.end(),
                                             object->__renderObject),
                                 scene->__renderObjects.end());

    delete object->__renderObject;
    object->__renderObject = 0;
  }

  void GLRenderer::updateObject(Object * object)
  {
    if (object->type() == Mesh::Type)
    {
      Mesh * mesh = dynamic_cast<Mesh *>(object);
      assert(mesh->geometry && "Mesh missing geometry buffer");

      Geometry * geom = mesh->geometry;
      GLObject * glObject = static_cast<GLObject *>(mesh->__renderObject);

      glObject->material = &mesh->material;

      // Has the geometry buffer changed?
      if (!geom->__renderGeometry || geom->__renderGeometry != glObject->geometry)
      {
        createGeometry(geom);
        glObject->geometry = static_cast<GLGeometry *>(geom->__renderGeometry);
      }

      updateGeometry(geom);
    }
  }

  void GLRenderer::updateGeometry(Geometry * geom)
  {
    GLGeometry * glGeom = static_cast<GLGeometry *>(geom->__renderGeometry);

    GLenum bufferUsage = geom->dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    if (geom->verticesNeedUpdate && glGeom->vertexBuffer)
    {
      glBindBuffer(GL_ARRAY_BUFFER, glGeom->vertexBuffer);
      glBufferData(GL_ARRAY_BUFFER, geom->vertices.size() * sizeof(Vector3), &geom->vertices[0], bufferUsage);
      geom->verticesNeedUpdate = false;
    }

    if (geom->normalsNeedUpdate && glGeom->normalBuffer)
    {
      glBindBuffer(GL_ARRAY_BUFFER, glGeom->normalBuffer);
      glBufferData(GL_ARRAY_BUFFER, geom->normals.size() * sizeof(Vector3), &geom->normals[0], bufferUsage);
      geom->normalsNeedUpdate = false;
    }

    if (geom->colorsNeedUpdate && glGeom->colorBuffer)
    {
      glBindBuffer(GL_ARRAY_BUFFER, glGeom->colorBuffer);
      glBufferData(GL_ARRAY_BUFFER, geom->colors.size() * sizeof(Color), &geom->colors[0], bufferUsage);
      geom->colorsNeedUpdate = false;
    }

    if (geom->texCoord0NeedUpdate && glGeom->texCoord0Buffer)
    {
      glBindBuffer(GL_ARRAY_BUFFER, glGeom->texCoord0Buffer);
      glBufferData(GL_ARRAY_BUFFER, geom->texCoord0.size() * sizeof(Vector2), &geom->texCoord0[0], bufferUsage);
      geom->texCoord0NeedUpdate = false;
    }

    if (geom->elementsNeedUpdate && glGeom->indexBuffer)
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glGeom->indexBuffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, geom->faces.size() * sizeof(Face), &geom->faces[0], bufferUsage);
      geom->elementsNeedUpdate = false;

      glGeom->faceCount = geom->faces.size() * 3;
    }
    else if (!glGeom->indexBuffer)
      glGeom->faceCount = geom->faceCount * 3;
  }

  void GLRenderer::updateMatrices(Object * object, Camera * camera)
  {
    GLObject * glObject = static_cast<GLObject *>(object->__renderObject);

    glObject->modelViewMatrix = camera->matrixWorldInverse * object->matrixWorld;
    glObject->normalMatrix = glObject->modelViewMatrix.inverse();
    glObject->normalMatrix.transpose();
  }

  void GLRenderer::createGeometry(Geometry * geometry)
  {
    if (geometry->__renderGeometry)
      return;

    GLGeometry * geom = new GLGeometry(geometry);

    if (!geometry->vertices.empty())
    {
      glGenBuffers(1, &geom->vertexBuffer);
      geometry->verticesNeedUpdate = true;
    }

    if (!geometry->normals.empty())
    {
      glGenBuffers(1, &geom->normalBuffer);
      geometry->normalsNeedUpdate = true;
    }

    if (!geometry->colors.empty())
    {
      glGenBuffers(1, &geom->colorBuffer);
      geometry->colorsNeedUpdate = true;
    }

    if (!geometry->texCoord0.empty())
    {
      glGenBuffers(1, &geom->texCoord0Buffer);
      geometry->texCoord0NeedUpdate = true;
    }

    if (!geometry->faces.empty())
    {
      glGenBuffers(1, &geom->indexBuffer);
      geometry->elementsNeedUpdate = true;
    }
  }

  void GLRenderer::createMaterial(Material * material, GLObject * object)
  {
    GLMaterial * glMat = static_cast<GLMaterial *>(material->__renderMaterial);

    // Do we need to generate a new program?
    if (glMat->program)
      glDeleteProgram(glMat->program);

    uint32_t vertexShader = compileShader(material->vertexShaderCode(), GL_VERTEX_SHADER);
    if (vertexShader == 0)
      return;

    uint32_t fragmentShader = compileShader(material->fragmentShaderCode(), GL_FRAGMENT_SHADER);
    if (fragmentShader == 0)
    {
      glDeleteShader(vertexShader);
      return;
    }

    glMat->program = glCreateProgram();
    glAttachShader(glMat->program, vertexShader);
    glAttachShader(glMat->program, fragmentShader);

    // Bind attributes
    glBindAttribLocation(glMat->program, POSITION_ATTRIB_LOCATION, "position");
    glBindAttribLocation(glMat->program, NORMAL_ATTRIB_LOCATION, "normal");
    glBindAttribLocation(glMat->program, UV0_ATTRIB_LOCATION, "uv0");
    glBindAttribLocation(glMat->program, UV1_ATTRIB_LOCATION, "uv1");
    glBindAttribLocation(glMat->program, COLOR_ATTRIB_LOCATION, "color");
    glBindAttribLocation(glMat->program, TANGENT_ATTRIB_LOCATION, "tangent");

    glLinkProgram(glMat->program);

    int ok;
    glGetProgramiv(glMat->program, GL_LINK_STATUS, &ok);
    if (!ok)
    {
      fprintf(stderr, "(%s): Failed to link shader program\n", material->name.c_str());
      showLogInfo(glMat->program, glGetProgramiv, glGetProgramInfoLog);

      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
      glDeleteProgram(glMat->program);
      glMat->program = 0;
      return;
    }

    glUseProgram(glMat->program);

    // Get material specific uniform positions
    for (uint32_t i = 0; i < material->uniformCount(); ++i)
    {
      int32_t location = glGetUniformLocation(glMat->program, material->uniformName(i));
      glMat->uniforms[i] = location;
    }

    // Get default uniform positions
    glMat->objectMatrix = glGetUniformLocation(glMat->program, "objectMatrix");
    glMat->modelViewMatrix = glGetUniformLocation(glMat->program, "modelViewMatrix");
    glMat->projectionMatrix = glGetUniformLocation(glMat->program, "projectionMatrix");
    glMat->viewMatrix = glGetUniformLocation(glMat->program, "viewMatrix");
    glMat->normalMatrix = glGetUniformLocation(glMat->program, "normalMatrix");
    glMat->cameraPosition = glGetUniformLocation(glMat->program, "cameraPosition");

    // Bind texture positions
    for (uint32_t i = 0; i < material->textureCount(); ++i)
    {
      GLuint pos = glGetUniformLocation(glMat->program, material->textureName(i));
      glUniform1i(pos, i);
    }

    glUseProgram(_currentProgram);
  }

}

