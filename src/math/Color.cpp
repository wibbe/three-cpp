
#include "math/Color.h"
#include <string.h>

namespace three {

  int fromHex(char character)
  {
    switch (character)
    {
      case '0':
        return 0;
      case '1':
        return 1;
      case '2':
        return 2;
      case '3':
        return 3;
      case '4':
        return 4;
      case '5':
        return 5;
      case '6':
        return 6;
      case '7':
        return 7;
      case '8':
        return 8;
      case '9':
        return 9;
      case 'a':
      case 'A':
        return 10;
      case 'b':
      case 'B':
        return 11;
      case 'c':
      case 'C':
        return 12;
      case 'd':
      case 'D':
        return 13;
      case 'e':
      case 'E':
        return 14;
      case 'f':
      case 'F':
        return 15;
    }

    return 0;
  }

  Color::Color(const char * hex)
    : r(1.0f),
      g(1.0f),
      b(1.0f),
      a(1.0f)
  {
    int len = strlen(hex);

    if (len == 7 && hex[0] == '#')
    {
      r = ((fromHex(hex[1]) << 4) + fromHex(hex[2])) / 255.0f;
      g = ((fromHex(hex[3]) << 4) + fromHex(hex[4])) / 255.0f;
      b = ((fromHex(hex[5]) << 4) + fromHex(hex[6])) / 255.0f;
    }
    else if (len == 4 && hex[0] == '#')
    {
      r = fromHex(hex[1]) / 15.0f;
      g = fromHex(hex[2]) / 15.0f;
      b = fromHex(hex[3]) / 15.0f;
    }
  }

}