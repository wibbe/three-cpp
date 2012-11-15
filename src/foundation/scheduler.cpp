
#include "scheduler.h"

#include "atomic.h"
#include "array.h"
#include "freelist.h"
#include "memory.h"

#include <pthread.h>

namespace foundation {

  namespace scheduler {

    struct Task
    {
      char _unusedFreeListAlias[sizeof(void *)];
      uint32_t _generation;
      Kernel _kernel;
      TaskData _data;
    };

    static Array<Task> * _taskPoolMemory = 0;
    static FreeList * _taskPool = 0;
    static pthread_mutex_t _taskPoolMutex;
    static AtomicInt _taskGeneration;

    // -- Internal --

    static uint32_t getTaskOffset(Task * task)
    {
      return task - array::begin(*_taskPoolMemory);
    }

    static Task * getTask(uint32_t offset)
    {
      return array::begin(*_taskPoolMemory) + offset;
    }

    static TaskRef getTaskRef(Task * task)
    {
      return TaskRef(getTaskOffset(task), task->_generation);
    }

    static Task * obtainTask()
    {
      Task * task = 0;

      pthread_mutex_lock(&_taskPoolMutex);
      task = static_cast<Task *>(freelist::obtain(*_taskPool));
      pthread_mutex_unlock(&_taskPoolMutex);

      task->_generation = atomic::inc(_taskGeneration);
      return task;
    }

    static void surrenderTask(Task * task)
    {
      task->_generation = atomic::inc(_taskGeneration);

      pthread_mutex_lock(&_taskPoolMutex);
      freelist::surrender(*_taskPool, task);
      pthread_mutex_unlock(&_taskPoolMutex);
    }

    static void queueTask(Task * task)
    {

    }

    // -- Interface --

    void init(Allocator & allocator, uint32_t taskCount, uint32_t workThreadCount)
    {
      _taskPoolMemory = MAKE_NEW(allocator, Array<Task>, allocator);
      _taskPool = MAKE_NEW(allocator, FreeList, array::begin(*_taskPoolMemory), array::end(*_taskPoolMemory), sizeof(Task));

      atomic::set(_taskGeneration, 0);
      pthread_mutex_init(&_taskPoolMutex, 0);
    }

    void shutdown()
    {
      if (_taskPoolMemory)
      {
        Allocator & allocator = *_taskPoolMemory->_allocator;

        MAKE_DELETE(allocator, Array<Task>, _taskPoolMemory);
        MAKE_DELETE(allocator, FreeList, _taskPool);

        _taskPoolMemory = 0;
        _taskPool = 0;
      }
    }

    TaskRef addTask(TaskData const& data, Kernel kernel)
    {
      Task * task = obtainTask();
      task->_kernel = kernel;
      task->_data = data;

      queueTask(task);
      return getTaskRef(task);
    }

    bool isTaskFinished(TaskRef const& taskRef)
    {
      Task * task = getTask(taskRef._offset);

      if (task->_generation != taskRef._generation)
      {
        // Task is from older generation and has been recycled, so it's finished
        return true;
      }

      return false;
    }

  }

}