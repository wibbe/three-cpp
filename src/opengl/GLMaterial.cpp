
#include "opengl/GLMaterial.h"

#include <GL/glew.h>
#include <cassert>

#include "base/Vector3.h"
#include "base/Vector4.h"
#include "base/Color.h"

namespace three {

  GLMaterial::GLMaterial(uint32_t _uniformCount)
    : program(0),
      uniforms(new int32_t[_uniformCount]),
      uniformCount(_uniformCount),
      objectMatrix(-1),
      modelViewMatrix(-1),
      projectionMatrix(-1),
      viewMatrix(-1),
      normalMatrix(-1),
      cameraPosition(-1)
  {
  }

  GLMaterial::~GLMaterial()
  {
    glDeleteProgram(program);
    delete[] uniforms;
  }

  void GLMaterial::uniform(uint32_t id, float value)
  {
    assert(id < uniformCount);
    glUniform1f(uniforms[id], value);
  }

  void GLMaterial::uniform(uint32_t id, Vector3 const& value)
  {
    assert(id < uniformCount);
    glUniform3f(uniforms[id], value.x, value.y, value.z);
  }

  void GLMaterial::uniform(uint32_t id, Vector4 const& value)
  {
    assert(id < uniformCount);
    glUniform4f(uniforms[id], value.x, value.y, value.z, value.w);
  }

  void GLMaterial::uniform(uint32_t id, Color const& value)
  {
    assert(id < uniformCount);
    glUniform4f(uniforms[id], value.r, value.g, value.b, value.a);
  }

}