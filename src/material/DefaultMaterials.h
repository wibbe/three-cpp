
#pragma once

#include "base/Material.h"

namespace three {

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
      uint32_t textureCount() const { return 3; }
      uint32_t uniformCount() const { return 7; }

    public:
      three::Texture * map;
      three::Texture * lightMap;
      three::Texture * envMap;
      three::Vector4 offsetRepeat;
      three::Color diffuse;
      float opacity;
      float flipEnvMap;
      int  useRefract;
      float reflectivity;
      float refractionRatio;

      bool useTextureMap;
      bool useLightMap;
      bool useEnvMap;
      bool doubleSided;
      bool useVertexColor;
      bool gammaCorrection;
      bool useReflectivity;
  };

  class MeshPhongMaterial : public three::Material
  {
    public:
      static uint32_t Type;

    public:
      MeshPhongMaterial();

      uint32_t type() const { return MeshPhongMaterial::Type; }

      void apply(Renderer * renderer);

      std::string vertexShaderCode() const;
      std::string fragmentShaderCode() const;

      const char * textureName(uint32_t slot) const;
      const char * uniformName(uint32_t slot) const;
      uint32_t textureCount() const { return 2; }
      uint32_t uniformCount() const { return 0; }

    public:
      three::Texture * map;
      three::Texture * lightMap;

  };

}
