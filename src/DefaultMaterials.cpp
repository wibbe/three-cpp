
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
    return three::Code::generate("basicVertexShader", 0);
  }

  std::string BasicColorMaterial::fragmentShaderCode() const
  {
    return three::Code::generate("basicFragmentShader", 0);
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
    return three::Code::generate("defaultVertexShader", "USE_MAP", 0);
  }

  std::string MeshBasicMaterial::fragmentShaderCode() const
  {
    return three::Code::generate("defaultFragmentShader", "USE_MAP", 0);
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
