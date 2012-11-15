
#pragma once

#include "foundation/collection_types.h"

namespace foundation {

  struct Stream
  {
    Stream(Allocator & allocator);
    Stream(const Stream & other);
    Stream & operator = (const Stream & other);

    Array<uint8_t> _data;
    uint32_t _position;
  };

}