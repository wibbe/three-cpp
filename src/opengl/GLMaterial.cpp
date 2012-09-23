
#include "opengl/GLMaterial.h"

#include <GL/glew.h>

#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"

namespace three {
  
  GLMaterial::GLMaterial(uint32_t _uniformCount)
    : program(0),
      uniforms(new uint32_t[_uniformCount]),
      uniformCount(_uniformCount),
      objectMatrix(0),
      modelViewMatrix(0),
      projectionMatrix(0),
      viewMatrix(0),
      normalMatrix(0),
      cameraPosition(0)
  {
  }

  GLMaterial::~GLMaterial()
  {
    glDeleteProgram(program);
    delete[] uniforms;
  }

  void GLMaterial::uniform(uint32_t id, float value)
  {
    glUniform1f(id, value);
  }

  void GLMaterial::uniform(uint32_t id, Vector3 const& value)
  {
    glUniform3f(id, value.x, value.y, value.z);
  }

  void GLMaterial::uniform(uint32_t id, Vector4 const& value)
  {
    glUniform4f(id, value.x, value.y, value.z, value.w);
  }

  void GLMaterial::uniform(uint32_t id, Color const& value)
  {
    glUniform4f(id, value.r, value.g, value.b, value.a);
  }

}