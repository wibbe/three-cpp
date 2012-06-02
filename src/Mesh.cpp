
#include "Mesh.h"
#include "StringHash.h"

namespace three {

  unsigned int Mesh::Type = StringHash("Mesh").hash;

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

