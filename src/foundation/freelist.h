
#pragma once

#include "foundation/freelist_types.h"

namespace foundation {

  namespace freelist
  {
    void * obtain(FreeList & list);
    void surrender(FreeList & list, void * ptr);
  }

  namespace freelist
  {
    inline void * obtain(FreeList & list)
    {
      FreeList * head = list._next;
      list._next = head->_next;
      return head;
    }

    inline void surrender(FreeList & list, void * ptr)
    {
      FreeList * head = static_cast<FreeList *>(ptr);
      head->_next = list._next;
      list._next = head;
    }
  }

}