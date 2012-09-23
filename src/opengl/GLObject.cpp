
#include "opengl/GLObject.h"
#include "opengl/GLGeometry.h"
#include "Object.h"
#include "Material.h"

namespace three {

  GLObject::GLObject(Object * source)
    : RenderObject(source),
      geometry(0),
      material(0),
      render(false)
  {
    source->__renderObject = this;
  }

}

