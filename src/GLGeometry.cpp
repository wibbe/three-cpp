
#include "GLGeometry.h"
#include "Geometry.h"

namespace three {

  GLGeometry::GLGeometry(Geometry * source)
    : sourceGeometry(source)
  {
    sourceGeometry->__renderObject = this;
  }

}

