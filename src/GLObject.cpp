
#include "GLObject.h"
#include "Object.h"

namespace three {

  GLObject::GLObject(Object * source)
    : sourceObject(source)
  {
    source->__renderObject = this;
  }

}

