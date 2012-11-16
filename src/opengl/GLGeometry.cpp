
#include "opengl/GLGeometry.h"
#include "base/Geometry.h"

namespace three {

  GLGeometry::GLGeometry(Geometry * source)
    : BackendGeometry(source),
      vertexBuffer(0),
      normalBuffer(0),
      colorBuffer(0),
      texCoord0Buffer(0),
      indexBuffer(0),
      faceCount(0)
  {
    sourceGeometry->__renderGeometry = this;
  }

}
