/**
 * Copyright (c) 2012 Daniel Wiberg
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

 #include <stdint.h>

namespace three {

  /// Compile time implementation of the FNV-1a hash algorithm
  template <unsigned int N, unsigned int I>
  struct FnvHash
  {
    inline static uint32_t hash(const char (&str)[N])
    {
      return (FnvHash<N, I - 1>::hash(str) ^ str[I - 1]) * 16777619u;
    }
  };

  template <unsigned int N>
  struct FnvHash<N, 1>
  {
    inline static uint32_t hash(const char (&str)[N])
    {
      return (2166136261u ^ str[0]) * 16777619u;
    }
  };

  /// Simple implementation of the FNV-1a hash algorithm
  inline uint32_t calculateFNV(const char * str)
  {
    const size_t length = strlen(str) + 1;
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < length; ++i)
    {
      hash ^= *str++;
      hash *= 16777619u;
    }
   
    return hash;
  }

  class StringHash
  {
    public:
      template <unsigned int N>
      StringHash(const char (&str)[N])
        : hash(FnvHash<N, N>::hash(str))
      { }

    public:
      uint32_t hash;
  };

}
