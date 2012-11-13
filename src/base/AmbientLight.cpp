
#include "base/AmbientLight.h"
#include "base/StringHash.h"

namespace three {

  uint32_t AmbientLight::Type = StringHash("AmbientLight").hash;

  AmbientLight::AmbientLight(Color const& _color)
    : Light(_color)
  { }

}
