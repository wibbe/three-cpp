
#pragma once

#include "foundation/atomic_types.h"

namespace foundation {
  namespace atomic
  {
    void set(AtomicInt & atomic, int value);
    int read(AtomicInt & atomic);
    int read(AtomicInt const& atomic);
    int add(AtomicInt & atomic, int value);
    int sub(AtomicInt & atomic, int value);

    /// Increases counter, and returns its value
    int inc(AtomicInt & atomic);

    /// Decreases counter, and returns its value
    int dec(AtomicInt & atomic);

    /// Compare and swap, if counter has value oldValue, then write newValue to counter.
    /// Returns true if comparison was successfull and new value was written.
    bool swap(AtomicInt & atomic, int oldValue, int newValue);
  }

  namespace {
    inline void set(AtomicInt & atomic, int value)
    {
      atomic._counter = value;
    }

    inline int read(AtomicInt & atomic)
    {
      return atomic._counter;
    }

    inline int read(AtomicInt const& atomic)
    {
      return atomic._counter;
    }

    inline int add(AtomicInt & atomic, int value)
    {
      return __sync_add_and_fetch(&atomic._counter, value);
    }

    inline int sub(AtomicInt & atomic, int value)
    {
      return __sync_sub_and_fetch(&atomic._counter, value);
    }

    inline int inc(AtomicInt & atomic)
    {
      return __sync_add_and_fetch(&atomic._counter, 1);
    }

    inline int dec(AtomicInt & atomic)
    {
      return __sync_sub_and_fetch(&atomic._counter, 1);
    }

    inline bool swap(AtomicInt & atomic, int oldValue, int newValue)
    {
      return __sync_bool_compare_and_swap(&atomic._counter, oldValue, newValue);
    }
  }
}