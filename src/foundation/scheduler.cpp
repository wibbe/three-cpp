
#include "scheduler.h"

#include "atomic.h"
#include "array.h"
#include "queue.h"
#include "freelist.h"
#include "memory.h"

#include <pthread.h>
#include <semaphore.h>

namespace foundation {

  namespace scheduler {

    #define NO_PARENT 0

    struct Task
    {
      char _unusedFreeListAlias[sizeof(void *)];
      AtomicInt _openTasks;
      uint32_t _parent;
      uint32_t _generation;
      Kernel _kernel;
      TaskData _data;
    };

    static Array<pthread_t> * _workerThreads = 0;
    static Array<Task> * _taskPoolMemory = 0;
    static FreeList * _taskPool = 0;
    static Queue<Task *> * _taskQueue = 0;
    static AtomicInt _taskGeneration;
    static bool _workerTheadsRunning = false;

    static pthread_mutex_t _taskPoolMutex;
    static pthread_mutex_t _taskQueueMutex;
    static sem_t _taskQueueSem;

    // -- Internal --

    static void workOnTask(Task * task);

    static uint32_t getTaskOffset(Task * task)
    {
      return task - array::begin(*_taskPoolMemory);
    }

    static Task * getTaskFromOffset(uint32_t offset)
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

      atomic::set(task->_openTasks, 1);
      task->_generation = atomic::inc(_taskGeneration);
      task->_kernel = 0;
      task->_parent = NO_PARENT;
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
      pthread_mutex_lock(&_taskQueueMutex);
      queue::push_back(*_taskQueue, task);
      pthread_mutex_unlock(&_taskQueueMutex);

      sem_post(&_taskQueueSem);
    }

    static Task * getAvailableTask()
    {
      Task * task = 0;
      pthread_mutex_lock(&_taskQueueMutex);

      if (queue::size(*_taskQueue) > 0)
      {
        task = *queue::begin_front(*_taskQueue);
        queue::consume(*_taskQueue, 1);
      }

      pthread_mutex_unlock(&_taskQueueMutex);

      return task;
    }

    static Task * waitUntilTaskIsAvailable()
    {
      sem_wait(&_taskQueueSem);
      return getAvailableTask();
    }

    static void finishTask(Task * task)
    {
      atomic::dec(task->_openTasks);

      if (task->_parent != NO_PARENT)
      {
        Task * parent = getTaskFromOffset(task->_parent);
        finishTask(parent);
      }

      if (atomic::read(task->_openTasks) == 0)
        surrenderTask(task);
    }

    static bool canExecuteTask(Task * task)
    {
      return atomic::read(task->_openTasks) == 1;
    }

    static void helpWithWork()
    {
      Task * task = getAvailableTask();
      if (task)
        workOnTask(task);
      else
      {
        #ifdef __APPLE__
          pthread_yield_np();
        #else
          pthread_yield();
        #endif
      }
    }

    static void workOnTask(Task * task)
    {
      while (!canExecuteTask(task))
        helpWithWork();

      if (task->_kernel)
        (task->_kernel)(task->_data);

      finishTask(task);
    }

    // The main function for all worker threads
    static void * work(void * data)
    {
      while (_workerTheadsRunning)
      {
        Task * task = waitUntilTaskIsAvailable();
        if (task)
          workOnTask(task);
      }

      return 0;
    }

    // -- Interface --

    void init(Allocator & allocator, uint32_t taskCount, uint32_t workThreadCount)
    {
      _workerThreads = MAKE_NEW(allocator, Array<pthread_t>, allocator);
      _taskPoolMemory = MAKE_NEW(allocator, Array<Task>, allocator);
      _taskPool = MAKE_NEW(allocator, FreeList, array::begin(*_taskPoolMemory), array::end(*_taskPoolMemory), sizeof(Task));
      _taskQueue = MAKE_NEW(allocator, Queue<Task *>, allocator);

      _workerTheadsRunning = true;

      atomic::set(_taskGeneration, 0);

      pthread_mutex_init(&_taskPoolMutex, 0);
      pthread_mutex_init(&_taskQueueMutex, 0);
      sem_init(&_taskQueueSem, 0, 0);

      // Create worker threads
      for (uint32_t i = 0; i < workThreadCount; ++i)
      {
        pthread_t thread;
        if (pthread_create(&thread, 0, &work, 0) == 0)
          array::push_back(*_workerThreads, thread);
      }
    }

    void shutdown()
    {
      // Join all work threads
      if (_workerThreads)
      {
        _workerTheadsRunning = false;

        // Increase semaphore with the number of threads, so we are sure they all wake up.
        for (uint32_t i = 0; i < array::size(*_workerThreads); ++i)
          sem_post(&_taskQueueSem);

        for (pthread_t * it = array::begin(*_workerThreads); it != array::end(*_workerThreads); ++it)
          pthread_join(*it, 0);

        array::clear(*_workerThreads);
      }

      if (_taskPoolMemory)
      {
        Allocator & allocator = *_taskPoolMemory->_allocator;

        MAKE_DELETE(allocator, Array<pthread_t>, _workerThreads);
        MAKE_DELETE(allocator, Array<Task>, _taskPoolMemory);
        MAKE_DELETE(allocator, FreeList, _taskPool);
        MAKE_DELETE(allocator, Queue<Task *>, _taskQueue);

        sem_destroy(&_taskQueueSem);

        _workerThreads = 0;
        _taskPoolMemory = 0;
        _taskPool = 0;
        _taskQueue = 0;
      }
    }

    TaskRef createAndExecuteTask(TaskData const& data, Kernel kernel)
    {
      TaskRef task = createTask(data, kernel);
      add(task);
      return task;
    }

    TaskRef createTask(TaskData const& data, Kernel kernel)
    {
      Task * task = obtainTask();
      task->_kernel = kernel;
      task->_data = data;

      return getTaskRef(task);
    }

    TaskRef createEmptyTask()
    {
      Task * task = obtainTask();
      task->_kernel = 0;
      return getTaskRef(task);
    }

    void add(TaskRef task)
    {
      queueTask(getTaskFromOffset(task._offset));
    }

    void link(TaskRef parentRef, TaskRef childRef)
    {
      Task * parent = getTaskFromOffset(parentRef._offset);
      Task * child = getTaskFromOffset(childRef._offset);

      atomic::inc(parent->_openTasks);
      child->_parent = parentRef._offset;
    }

    bool isTaskFinished(TaskRef const& taskRef)
    {
      Task * task = getTaskFromOffset(taskRef._offset);

      if (task->_generation != taskRef._generation)
      {
        // Task is from older generation and has been recycled, so it's finished
        return true;
      }

      return false;
    }

    void wait(TaskRef const& task)
    {
      while (!isTaskFinished(task))
        helpWithWork();
    }

  }

}