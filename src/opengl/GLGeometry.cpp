
#include "GLGeometry.h"
#include "Geometry.h"

namespace three {

  GLGeometry::GLGeometry(Geometry * source)
    : RenderGeometry(source)
  {
    sourceGeometry->__renderGeometry = this;
  }

}

