
#pragma once

#include "stream_types.h"
#include "array.h"

#include <cassert>

namespace foundation {

  // -- Interface --
  namespace stream {
    void rewind(Stream & s);
    bool eof(Stream & s);

    uint8_t * begin(Stream & s);
    uint8_t const * begin(Stream const & s);
    uint8_t * end(Stream & s);
    uint8_t const * end(Stream const & s);

    template <typename T> Stream & push(Stream & s, T * data, uint32_t num);
    template <typename T> Stream & read(Stream & s, T * data, uint32_t num);
  }

  Stream & operator << (Stream & s, int8_t value);
  Stream & operator << (Stream & s, uint8_t value);
  Stream & operator << (Stream & s, int16_t value);
  Stream & operator << (Stream & s, uint16_t value);
  Stream & operator << (Stream & s, int32_t value);
  Stream & operator << (Stream & s, uint32_t value);
  Stream & operator << (Stream & s, float value);
  Stream & operator << (Stream & s, double value);

  Stream & operator >> (Stream & s, int8_t & value);
  Stream & operator >> (Stream & s, uint8_t & value);
  Stream & operator >> (Stream & s, int16_t & value);
  Stream & operator >> (Stream & s, uint16_t & value);
  Stream & operator >> (Stream & s, int32_t & value);
  Stream & operator >> (Stream & s, uint32_t & value);
  Stream & operator >> (Stream & s, float & value);
  Stream & operator >> (Stream & s, double & value);

  // -- Implementation --

  inline Stream & operator << (Stream & s, int8_t value)
  {
    stream::push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, uint8_t value)
  {
    stream::push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, int16_t value)
  {
    stream::push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, uint16_t value)
  {
    stream::push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, int32_t value)
  {
    stream::push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, uint32_t value)
  {
    stream::push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, float value)
  {
    stream::push(s, &value, 1);
    return s;
  }

  inline Stream & operator << (Stream & s, double value)
  {
    stream::push(s, &value, 1);
    return s;
  }

  inline Stream & operator >> (Stream & s, int8_t & value)
  {
    stream::read(s, &value, 1);
    return s;
  }

  inline Stream & operator >> (Stream & s, uint8_t & value)
  {
    stream::read(s, &value, 1);
    return s;
  }

  inline Stream & operator >> (Stream & s, int16_t & value)
  {
    stream::read(s, &value, 1);
    return s;
  }

  inline Stream & operator >> (Stream & s, uint16_t & value)
  {
    stream::read(s, &value, 1);
    return s;
  }

  inline Stream & operator >> (Stream & s, int32_t & value)
  {
    stream::read(s, &value, 1);
    return s;
  }

  inline Stream & operator >> (Stream & s, uint32_t & value)
  {
    stream::read(s, &value, 1);
    return s;
  }

  inline Stream & operator >> (Stream & s, float & value)
  {
    stream::read(s, &value, 1);
    return s;
  }

  inline Stream & operator >> (Stream & s, double & value)
  {
    stream::read(s, &value, 1);
    return s;
  }

  namespace stream {

    template <typename T> inline Stream & push(Stream & s, T * data, uint32_t num)
    {
      array::resize(s._data, s._position + (num + sizeof(T)));
      memcpy(array::begin(s._data) + s._position, data, (num * sizeof(T)));
      s._position += (num * sizeof(T));
      return s;
    }

    template <typename T> inline Stream & read(Stream & s, T * data, uint32_t num)
    {
      assert((s._position + (sizeof(T) * num)) <= array::size(s._data));
      memcpy(data, array::begin(s._data) + s._position, (num * sizeof(T)));
      s._position += (num * sizeof(T));
      return s;
    }

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