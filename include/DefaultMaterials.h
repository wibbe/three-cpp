
#pragma once

#include "Material.h"

namespace three {

  class BasicColorMaterial : public three::Material
  {
    public:
      static uint32_t Type;

    public:
      BasicColorMaterial();

      uint32_t type() const { return BasicColorMaterial::Type; }

      void apply(Renderer * renderer);

      std::string vertexShaderCode() const;
      std::string fragmentShaderCode() const;

      const char * textureName(uint32_t slot) const;
      const char * uniformName(uint32_t slot) const;
      uint32_t textureCount() const { return 0; }
      uint32_t uniformCount() const { return 1; }

    public:
      three::Color color;
  };

  class MeshBasicMaterial : public three::Material
  {
    public:
      static uint32_t Type;

    public:
      MeshBasicMaterial();

      uint32_t type() const { return MeshBasicMaterial::Type; }

      void apply(Renderer * renderer);

      std::string vertexShaderCode() const;
      std::string fragmentShaderCode() const;

      const char * textureName(uint32_t slot) const;
      const char * uniformName(uint32_t slot) const;
      uint32_t textureCount() const { return 1; }
      uint32_t uniformCount() const { return 3; }

    public:
      three::Texture * map;
      three::Vector4 offsetRepeat;
      three::Color diffuse;
      float opacity;
  };

}
