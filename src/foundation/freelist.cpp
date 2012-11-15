
#include "freelist.h"

namespace foundation {

  FreeList::FreeList(void * start, void * end, uint32_t elementSize)
    : _next(0)
  {
    union
    {
      void * asVoid;
      char * asChar;
      FreeList * asSelf;
    };

    asVoid = start;
    _next = asSelf;

    const uint32_t numElements = (static_cast<char *>(end) - asChar) / elementSize;

    asChar += elementSize;

    // Initialize the free list
    FreeList * runner = _next;
    for (uint32_t i = 1; i < numElements; ++i)
    {
      runner->_next = asSelf;
      runner = runner->_next;

      asChar += elementSize;
    }

    runner->_next = 0;
  }

}