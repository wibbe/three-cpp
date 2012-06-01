
#include "Material.h"

namespace three {

  static int materialCount = 1;

  Material::Material()
    : name(""),
      materialIndex(materialCount++),
      opacity(1.0f),
      transparent(false),
      blending(NormalBlending),
      depthTest(true),
      depthWrite(true),
      alphaTest(0),
      visible(true),
      needsUpdate(true)
  {
  }


}

