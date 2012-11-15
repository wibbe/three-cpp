
#pragma once

#include "foundation/types.h"

namespace foundation {

  struct TaskData;

  typedef void (*Kernel)(TaskData const& data);

  struct TaskRef
  {
    uint32_t _offset;
    uint32_t _generation;

    TaskRef(uint32_t offset, uint32_t generation)
      : _offset(offset),
        _generation(generation)
    { }
  };

  struct TaskData
  {
    void * data;

    union
    {
      struct
      {
        uint32_t elementCount;
        void * inputStreams[4];
        void * outputStreams[4];
      } streamingData;
    };
  };
}