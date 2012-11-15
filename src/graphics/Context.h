
#pragma once

#include "foundation/array.h"
#include "graphics/Context_types.h"

namespace graphics {

  // -- Interface --
  namespace command {
    Buffer & operator << (Buffer & b, int32_t value);
    Buffer & operator << (Buffer & b, uint32_t value);
    Buffer & operator << (Buffer & b, float value);
    Buffer & operator << (Buffer & b, Command cmd);

    template <typename T> Buffer & push(Buffer & b, T * data, uint32_t num);
  }

  // -- Implementation --
  namespace command {
    inline Buffer & operator << (Buffer & b, int32_t value)
    {
      push(b, &value, 1);
    }

    inline Buffer & operator << (Buffer & b, uint32_t value)
    {
      push(b, &value, 1);
    }

    inline Buffer & operator << (Buffer & b, float value)
    {
      push(b, &value, 1);
    }

    inline Buffer & operator << (Buffer & b, Command cmd)
    {
      push(b, &cmd, 1);
    }

    template <typename T> inline Buffer & push(Buffer &b, T * data, uint32_t num)
    {
      unsigned int end = foundation::array::size(b);
      foundation::array::resize(b, end + (num + sizeof(T)));
      memcpy(foundation::array::begin(b) + end, data, (num * sizeof(T)));
      return b;
    }
  }
}