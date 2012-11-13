
#include "base/Mesh.h"
#include "base/StringHash.h"

namespace three {

  uint32_t Mesh::Type = StringHash("Mesh").hash;

  Mesh::Mesh()
    : Object(),
      geometry(0),
      material(0)
  {
  }

  Mesh::Mesh(Geometry * _geometry, Material * _material)
    : Object(),
      geometry(_geometry),
      material(_material)
  {
  }

}

