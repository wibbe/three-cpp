
#pragma once

#include "stream_types.h"
#include "array.h"

namespace foundation {

  // -- Interface --
  namespace stream {
    void rewind(Stream & s);
    bool eof(Stream & s);

    uint8_t * begin(Stream & s);
    uint8_t const * begin(Stream const & s);
    uint8_t * end(Stream & s);
    uint8_t const * end(Stream const & s);
  }

  Stream & operator << (Stream & s, int8_t value);
  Stream & operator << (Stream & s, uint8_t value);
  Stream & operator << (Stream & s, int16_t value);
  Stream & operator << (Stream & s, uint16_t value);
  Stream & operator << (Stream & s, int32_t value);
  Stream & operator << (Stream & s, uint32_t value);
  Stream & operator << (Stream & s, float value);
  Stream & operator << (Stream & s, double value);

  template <typename T> Stream & push(Stream & s, T * data, uint32_t num);

  // -- Implementation --

  inline Stream & operator << (Stream & s, int8_t value)
  {
    push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, uint8_t value)
  {
    push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, int16_t value)
  {
    push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, uint16_t value)
  {
    push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, int32_t value)
  {
    push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, uint32_t value)
  {
    push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, float value)
  {
    push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, double value)
  {
    push(s, &value, 1);
    return s;
  }

  template <typename T> inline Stream & push(Stream & s, T * data, uint32_t num)
  {
    array::resize(s._data, s._position + (num + sizeof(T)));
    memcpy(array::begin(s._data) + s._position, data, (num * sizeof(T)));
    s._position += (num * sizeof(T));
    return s;
  }

  namespace stream {
    inline void rewind(Stream & s)
    {
      s._position = 0;
    }

    inline bool eof(Stream & s)
    {
      return s._position >= array::size(s._data);
    }

    inline uint8_t * begin(Stream & s)
    {
      return array::begin(s._data);
    }

    inline uint8_t const * begin(Stream const & s)
    {
      return array::begin(s._data);
    }

    inline uint8_t * end(Stream & s)
    {
      return array::begin(s._data) + s._position;
    }

    inline uint8_t const * end(Stream const & s)
    {
      return array::begin(s._data) + s._position;
    }
  }

}