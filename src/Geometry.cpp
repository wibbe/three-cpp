
#include "Geometry.h"

namespace three {

  Geometry::Geometry()
    : hasTangents(false),
      dynamic(false),
      boundingSphereRadius(-1.0f),
      verticesNeedUpdate(false),
      normalsNeedUpdate(false),
      elementsNeedUpdate(false)
  {
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

