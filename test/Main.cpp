
#include "Test.h"

int main(int argc, char * argv[])
{
  if (argc == 2)
    return test::runTest(argv[1]);
  else
    return test::runAllTests();
}