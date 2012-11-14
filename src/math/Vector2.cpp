
#include "math/Vector2.h"
#include "math/Matrix4.h"

#include <stdio.h>

namespace three {

  std::string Vector2::str() const
  {
    char str[1024];
    snprintf(str, 1024, "[ %3.1f, %3.1f ]", x, y);
    return std::string(str);
  }

}

