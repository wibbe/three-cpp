
#include "GLObject.h"
#include "Object.h"

namespace three {

  GLObject::GLObject(Object * source)
    : RenderObject(source),
      render(false)
  {
    source->__renderObject = this;
  }

}

