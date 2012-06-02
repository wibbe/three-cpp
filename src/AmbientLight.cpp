
#include "AmbientLight.h"
#include "StringHash.h"

namespace three {

  unsigned int AmbientLight::Type = StringHash("AmbientLight").hash();

  AmbientLight::AmbientLight(Color const& _color)
    : Light(_color)
  { }

  unsigned int AmbientLight::type() const
  {
    return AmbientLight::Type;
  }

}

