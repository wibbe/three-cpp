
#pragma once

#include "foundation/types.h"

namespace foundation {

  struct FreeList
  {
    FreeList(void * start, void * end, uint32_t elementSize);

    FreeList * _next;
  };

}