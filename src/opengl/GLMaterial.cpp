
#include "opengl/GLMaterial.h"

namespace three {
  
  GLMaterial::GLMaterial(uint32_t _uniformCount)
    : program(0),
      uniforms(new uint32_t[_uniformCount]),
      uniformCount(_uniformCount)
  {
  }

  GLMaterial::~GLMaterial()
  {
    delete[] uniforms;
  }

  void GLMaterial::uniform(uint32_t id, float value)
  {
  }

  void GLMaterial::uniform(uint32_t id, Vector3 const& value)
  {
  }

  void GLMaterial::uniform(uint32_t id, Vector4 const& value)
  {
  }

  void GLMaterial::uniform(uint32_t id, Color const& value)
  {
  }

}