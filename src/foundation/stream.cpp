
#include "stream.h"

namespace foundation {

  Stream::Stream(Allocator & allocator)
    : _data(allocator),
      _position(0)
  { }

  Stream::Stream(Stream const& copy)
    : _data(copy._data),
      _position(copy._position)
  { }

  Stream & Stream::operator = (Stream const& copy)
  {
    _data = copy._data;
    _position = copy._position;
    return *this;
  }

}