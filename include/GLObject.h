
#pragma once

#include "Matrix4.h"

namespace three {

  // Forward declarations
  class Object;

  class GLObject
  {
    public:
      GLObject(Object * source);

    public:
      Object * sourceObject;

      Matrix4 modelViewMatrix;
      Matrix4 normalMatrix;
  };

}

