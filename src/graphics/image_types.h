
#pragma once

namespace tree {
  struct Image
  {
    uint16_t width;
    uint16_t height;
    uint8_t format;
    uint8_t type;
    uint8_t * _data;
  };
}