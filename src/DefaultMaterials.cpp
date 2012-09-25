
#include "DefaultMaterials.h"
#include "Code.h"
#include "RenderMaterial.h"
#include "StringHash.h"
#include "Renderer.h"
#include <cassert>

namespace three {


  uint32_t MeshBasicMaterial::Type = three::StringHash("MeshBasicMaterial").hash;

  MeshBasicMaterial::MeshBasicMaterial()
  {
    map = 0;
    lightMap = 0;
    envMap = 0;
    combine = three::MixOperation;
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
  }

  void MeshBasicMaterial::apply(Renderer * renderer)
  {
    assert(__renderMaterial && renderer);
    __renderMaterial->uniform(0, combine);
    __renderMaterial->uniform(1, offsetRepeat);
    __renderMaterial->uniform(2, diffuse);
    __renderMaterial->uniform(3, opacity);
    __renderMaterial->uniform(4, flipEnvMap);
    __renderMaterial->uniform(5, useRefract);
    __renderMaterial->uniform(6, reflectivity);
    __renderMaterial->uniform(7, refractionRatio);
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
        return "combine";
      case 1:
        return "offsetRepeat";
      case 2:
        return "diffuse";
      case 3:
        return "opacity";
      case 4:
        return "flipEnvMap";
      case 5:
        return "useRefract";
      case 6:
        return "reflectivity";
      case 7:
        return "refractionRatio";
      default:
        return 0;
    }
  }

}
