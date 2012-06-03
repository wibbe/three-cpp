
#include "MeshBasicMaterial.h"
#include "StringHash.h"

namespace three {

  unsigned int MeshBasicMaterial::Type = StringHash("MeshBasicMaterial").hash;

  MeshBasicMaterial::MeshBasicMaterial(std::string const & _map)
    : Material(),
      map(_map)
  {
  }
}

