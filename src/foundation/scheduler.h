
#pragma once

#include "foundation/scheduler_types.h"
#include "foundation/memory_types.h"

namespace foundation {
  namespace scheduler
  {
    void init(Allocator & allocator, uint32_t taskCount, uint32_t workThreadCount);
    void shutdown();

    TaskRef addTask(TaskData const& data, Kernel kernel);

    bool isTaskFinished(TaskRef const& task);
  }
}