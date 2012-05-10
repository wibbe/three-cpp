
#include "Material.h"

namespace three {

  static int materialCount = 1;

  Material::Material()
    : name(""),
      id(materialCount++),
      opacity(1.0f),
      transparent(false),
      blending(NormaBlending),
      depthTest(true),
      depthWrite(true),
      alphaTest(0),
      visible(true),
      needsUpdate(true)
  {
  }


}

