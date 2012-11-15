
#pragma once

#include "foundation/scheduler_types.h"
#include "foundation/memory_types.h"

namespace foundation {
  namespace scheduler
  {
    void init(Allocator & allocator, uint32_t taskCount, uint32_t workThreadCount);
    void shutdown();

    TaskRef createAndExecuteTask(TaskData const& data, Kernel kernel);

    TaskRef createTask(TaskData const& data, Kernel kernel);
    TaskRef createEmptyTask();
    void add(TaskRef task);

    /// Link two tasks together. Parent task will wait until all of it's child tasks have executed
    void link(TaskRef parent, TaskRef child);

    bool isTaskFinished(TaskRef const& task);
    void wait(TaskRef const& task);
  }
}