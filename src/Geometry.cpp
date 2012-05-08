
#include "Geometry.h"

namespace three {

  Geometry::Geometry()
    : hasTangents(false),
      dynamic(false),
      boundingSphereRadius(-1.0f)
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

  void Geometry::computeBoundingSphere()
  {
    boundingSphereRadius = 0.0f;

    for (VertexArray::const_iterator it = vertices.begin(), end = vertices.end(); it != end; ++it)
    {
      float radius = (*it).length();
      if (radius > boundingSphereRadius)
        boundingSphereRadius = radius;
    }
  }

}

