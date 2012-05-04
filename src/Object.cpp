
#include "Object.h"

namespace three {

  Object::Object()
    : parent(0),
      position(),
      rotation(),
      scale(1.0f, 1.0f, 1.0f),
      boundRadius(1.0f),
      visible(true)
  {
  }

  Object::~Object()
  {
  }

}

