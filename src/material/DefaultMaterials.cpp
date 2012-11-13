
#include "DefaultMaterials.h"
#include "base/Code.h"
#include "base/RenderMaterial.h"
#include "base/StringHash.h"
#include "base/Renderer.h"
#include <cassert>

namespace three {


  uint32_t MeshBasicMaterial::Type = three::StringHash("MeshBasicMaterial").hash;

  MeshBasicMaterial::MeshBasicMaterial()
  {
    map = 0;
    lightMap = 0;
    envMap = 0;
    offsetRepeat = three::Vector4(0, 0, 1, 1);
    diffuse = three::Color(1, 1, 1, 1);
    opacity = 1;
    flipEnvMap = -1;
    useRefract = 0;
    reflectivity = 1.0;
    refractionRatio = 0.98;
    useTextureMap = false;
    useLightMap = false;
    useEnvMap = false;
    doubleSided = false;
    useVertexColor = false;
    gammaCorrection = false;
    useReflectivity = true;
  }

  void MeshBasicMaterial::apply(Renderer * renderer)
  {
    assert(__renderMaterial && renderer);
    __renderMaterial->uniform(0, offsetRepeat);
    __renderMaterial->uniform(1, diffuse);
    __renderMaterial->uniform(2, opacity);
    __renderMaterial->uniform(3, flipEnvMap);
    __renderMaterial->uniform(4, useRefract);
    __renderMaterial->uniform(5, reflectivity);
    __renderMaterial->uniform(6, refractionRatio);
    renderer->setTexture(map, 0);
    renderer->setTexture(lightMap, 1);
    renderer->setTexture(envMap, 2);
  }

  std::string MeshBasicMaterial::vertexShaderCode() const
  {
    std::vector<std::string> defines;
    if (useTextureMap)
      defines.push_back("USE_MAP");
    if (useLightMap)
      defines.push_back("USE_LIGHTMAP");
    if (useEnvMap)
      defines.push_back("USE_ENVMAP");
    if (doubleSided)
      defines.push_back("DOUBLE_SIDED");
    if (useVertexColor)
      defines.push_back("USE_COLOR");
    if (gammaCorrection)
      defines.push_back("USE_GAMMA");
    if (useReflectivity)
      defines.push_back("USE_REFLECTIVITY");

    return three::Code::generate("basicVertexShader", defines);
  }

  std::string MeshBasicMaterial::fragmentShaderCode() const
  {
    std::vector<std::string> defines;
    if (useTextureMap)
      defines.push_back("USE_MAP");
    if (useLightMap)
      defines.push_back("USE_LIGHTMAP");
    if (useEnvMap)
      defines.push_back("USE_ENVMAP");
    if (doubleSided)
      defines.push_back("DOUBLE_SIDED");
    if (useVertexColor)
      defines.push_back("USE_COLOR");
    if (gammaCorrection)
      defines.push_back("USE_GAMMA");
    if (useReflectivity)
      defines.push_back("USE_REFLECTIVITY");

    return three::Code::generate("basicFragmentShader", defines);
  }

  const char * MeshBasicMaterial::textureName(uint32_t slot) const
  {
    switch (slot)
    {
      case 0:
        return "map";
      case 1:
        return "lightMap";
      case 2:
        return "envMap";
      default:
        return 0;
    }
  }

  const char * MeshBasicMaterial::uniformName(uint32_t slot) const
  {
    switch (slot)
    {
      case 0:
        return "offsetRepeat";
      case 1:
        return "diffuse";
      case 2:
        return "opacity";
      case 3:
        return "flipEnvMap";
      case 4:
        return "useRefract";
      case 5:
        return "reflectivity";
      case 6:
        return "refractionRatio";
      default:
        return 0;
    }
  }

}
