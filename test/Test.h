
#pragma once

#include <stdio.h>

namespace test {

  class Test
  {
    public:
      Test(const char * name);

      virtual void run() = 0;

      Test * next() const { return m_next; }
      const char * name() const { return m_name; }

    private:
      const char * m_name;
      Test * m_next;
  };

  class Exception
  {
    public:
      Exception(const char * str, ...);
      const char * message() const { return m_message; }

    private:
      char m_message[128];
  };

  int runAllTests();
  int runTest(const char * name);
}


#define TEST(group, name) class group##_##name##_Test : test::Test \
                          { \
                            public: \
                              group##_##name##_Test(const char * name) \
                                : Test(name) \
                              { } \
                              void run(); \
                          }; \
                          static group##_##name##_Test __test_##group##_##name##__(#group"::"#name); \
                          void group##_##name##_Test::run()

#define CHECK(expr) do { \
                      if (!(expr)) throw test::Exception("%s(%d): %s", __FILE__, __LINE__, #expr); \
                    } while (0)

#define CHECK_TRUE(expr) do { \
                           if (!(expr)) throw test::Exception("%s(%d): %s", __FILE__, __LINE__, #expr); \
                         } while (0)

#define CHECK_FALSE(expr) do { \
                            if (expr) throw test::Exception("%s(%d): %s", __FILE__, __LINE__, #expr); \
                          } while (0)

#define CHECK_EQ(arg, expected) do {                                                         \
                                  if ((arg) != (expected)) throw test::Exception("%s(%d): %s == %s", __FILE__, __LINE__, #arg, #expected);  \
                                } while (0)

