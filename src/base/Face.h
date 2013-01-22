
#pragma once

#include <stdint.h>
#include "base/Vector3.h"

namespace three {

  class Face
  {
    public:
      Face()
      { }

      Face(uint16_t _a, uint16_t _b, uint16_t _c)
        : a(_a),
          b(_b),
          c(_c)
      { }

    public:
      uint16_t a, b, c;
  };

}

