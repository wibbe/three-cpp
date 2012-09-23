
#include "opengl/GLObject.h"
#include "opengl/GLGeometry.h"
#include "Object.h"
#include "Material.h"

namespace three {

  GLObject::GLObject(Object * source)
    : RenderObject(source),
      buffer(0),
      transparentMaterial(0),
      opaqueMaterial(0),
      render(false)
  {
    source->__renderObject = this;
  }

}

