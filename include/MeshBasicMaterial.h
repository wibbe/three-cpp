
#pragma once

#include "Material.h"

namespace three {

  class MeshBasicMaterial : public three::Material
  {
    public:
      static uint32_t Type;

    public:
      MeshBasicMaterial();

      uint32_t type() const { return MeshBasicMaterial::Type; }

      void apply();

      std::string vertexShaderCode() const;
      std::string fragmentShaderCode() const;

      const char * textureName(uint32_t slot) const;
      const char * uniformName(uint32_t slot) const;
      uint32_t textureCount() const { return 1; }
      uint32_t uniformCount() const { return 1; }

    public:
      three::Texture * map;
      three::Vector4 offsetRepeat;
  };

}
