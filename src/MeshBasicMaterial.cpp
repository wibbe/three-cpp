
#include "MeshBasicMaterial.h"
#include "Code.h"
#include "RenderMaterial.h"
#include "StringHash.h"
#include <cassert>

namespace three {


  uint32_t MeshBasicMaterial::Type = three::StringHash("MeshBasicMaterial").hash;

  MeshBasicMaterial::MeshBasicMaterial()
  {
    map = 0;
    offsetRepeat = three::Vector4(0, 0, 1, 1);
  }

  void MeshBasicMaterial::apply()
  {
    assert(__renderMaterial);
    __renderMaterial->uniform(0, offsetRepeat);
  }

  std::string MeshBasicMaterial::vertexShaderCode() const
  {
    return three::Code::generate("basicVertex", "USE_COLOR", "USE_MAP", 0);
  }

  std::string MeshBasicMaterial::fragmentShaderCode() const
  {
    return three::Code::generate("basicFragment", "USE_COLOR", "USE_MAP", 0);
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
      default:
        return 0;
    }
  }

}
