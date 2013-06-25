
#include "opengl/GLObject.h"
#include "opengl/GLGeometry.h"
#include "base/Object.h"
#include "base/Material.h"

namespace three {

  GLObject::GLObject(Object * source)
    : BackendObject(source),
      geometry(0),
      material(0)
  {
    source->__renderObject = this;
  }

}

