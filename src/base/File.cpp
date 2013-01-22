
#include <stdio.h>

namespace three {

  bool fileContent(const char * filename, unsigned char ** data, int & length)
  {
    FILE * file = fopen(filename, "r");

    if (!file)
      return false;

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    rewind(file);

    *data = new unsigned char[length];

    fread(*data, sizeof(char), length, file);
    fclose(file);

    return true;
  }

}