
#include "DefaultMaterials.h"
#include "Code.h"
#include "RenderMaterial.h"
#include "StringHash.h"
#include "Renderer.h"
#include <cassert>

namespace three {


  uint32_t BasicColorMaterial::Type = three::StringHash("BasicColorMaterial").hash;

  BasicColorMaterial::BasicColorMaterial()
  {
    color = three::Color(1, 1, 1, 1);
  }

  void BasicColorMaterial::apply(Renderer * renderer)
  {
    assert(__renderMaterial && renderer);
    __renderMaterial->uniform(0, color);
  }

  std::string BasicColorMaterial::vertexShaderCode() const
  {
    std::vector<std::string> defines;

    return three::Code::generate("basicVertexShader", defines);
  }

  std::string BasicColorMaterial::fragmentShaderCode() const
  {
    std::vector<std::string> defines;

    return three::Code::generate("basicFragmentShader", defines);
  }

  const char * BasicColorMaterial::textureName(uint32_t slot) const
  {
    switch (slot)
    {
      default:
        return 0;
    }
  }

  const char * BasicColorMaterial::uniformName(uint32_t slot) const
  {
    switch (slot)
    {
      case 0:
        return "color";
      default:
        return 0;
    }
  }


  uint32_t MeshBasicMaterial::Type = three::StringHash("MeshBasicMaterial").hash;

  MeshBasicMaterial::MeshBasicMaterial()
  {
    map = 0;
    offsetRepeat = three::Vector4(0, 0, 1, 1);
    diffuse = three::Color(1, 1, 1, 1);
    opacity = 1;
    useTextureMap = true;
    useVertexColor = false;
  }

  void MeshBasicMaterial::apply(Renderer * renderer)
  {
    assert(__renderMaterial && renderer);
    __renderMaterial->uniform(0, offsetRepeat);
    __renderMaterial->uniform(1, diffuse);
    __renderMaterial->uniform(2, opacity);
    renderer->setTexture(map, 0);
  }

  std::string MeshBasicMaterial::vertexShaderCode() const
  {
    std::vector<std::string> defines;
    if (useTextureMap)
      defines.push_back("USE_MAP");
    if (useVertexColor)
      defines.push_back("USE_COLOR");

    return three::Code::generate("defaultVertexShader", defines);
  }

  std::string MeshBasicMaterial::fragmentShaderCode() const
  {
    std::vector<std::string> defines;
    if (useTextureMap)
      defines.push_back("USE_MAP");
    if (useVertexColor)
      defines.push_back("USE_COLOR");

    return three::Code::generate("defaultFragmentShader", defines);
  }

  const char * MeshBasicMaterial::textureName(uint32_t slot) const
  {
    switch (slot)
    {
      case 0:
        return "map";
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
      default:
        return 0;
    }
  }

}
