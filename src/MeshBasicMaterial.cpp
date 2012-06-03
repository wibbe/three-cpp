
#include "MeshBasicMaterial.h"
#include "StringHash.h"

namespace three {

  unsigned int MeshBasicMaterial::Type = StringHash("MeshBasicMaterial").hash;

  MeshBasicMaterial::MeshBasicMaterial(Texture * _map)
    : Material(),
      map(_map)
  {
  }
}

