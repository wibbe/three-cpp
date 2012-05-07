
#include "Geometry.h"

namespace three {

  Geometry::Geometry()
    : hasTangents(false),
      dynamic(false)
  {
  }

  void Geometry::computeCentroids()
  {
    for (FaceArray::iterator it = faces.begin(), end = faces.end(); it != end; ++it)
    {
      Face & face = *it;

      face.centroid = (vertices[face.a] + vertices[face.b] + vertices[face.c]) * 0.333333f;
    }
  }

}

