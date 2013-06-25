
#include "Test.h"

#include <stdarg.h>
#include <string.h>

namespace test {

  namespace {
    static Test * _testHead = NULL;
    static Test * _testTail = NULL;
  }

  Test::Test(const char * name)
    : m_name(name),
      m_next(NULL)
  {
    if (_testHead)
    {
      _testTail->m_next = this;
      _testTail = this;
    }
    else
    {
      _testHead = this;
      _testTail = this;
    }
  }

  Exception::Exception(const char * str, ...)
  {
    va_list varArg;
    va_start(varArg, str);
    vsnprintf(m_message, 128, str, varArg);
    va_end(varArg);
  }

  int runAllTests()
  {
    bool failed = false;
    int totalCount = 0;
    for (Test * it = _testHead; it; it = it->next())
      totalCount++;

    int i = 1;
    for (Test * it = _testHead; it; it = it->next())
    {
      printf("[%d/%d] %s - ", i, totalCount, it->name());

      try
      {
        it->run();
        printf("SUCCESS\n");
      }
      catch (Exception const& exception)
      {
        printf("FAILED\n");
        printf("  %s\n", exception.message());
        failed = true;
      }

      i++;
    }

    return failed ? 1 : 0;
  }

  int runTest(const char * name)
  {
    for (Test * it = _testHead; it; it = it->next())
    {
      if (strcmp(it->name(), name) == 0)
      {
        printf("[1/1] %s - ", it->name());

        try
        {
          it->run();
          printf("SUCCESS\n");
        }
        catch (Exception const& exception)
        {
          printf("FAILED\n");
          printf("  %s\n", exception.message());
          return 1;
        }

        return 0;
      }
    }

    printf("Could not fint test '%s'\n", name);
    return 1;
  }

}