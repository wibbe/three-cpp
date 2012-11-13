
#include <stdint.h>
#include <cassert>

namespace three {

  uint32_t murmurHash(void const * key, int len, uint32_t seed)
  {
    // 'magic' and 'r' are mixing constants generated offline.
    // They're not really 'magic', they just happen to work well.
    const uint32_t magic = 0x5bd1e995;
    const int r = 24;

    // Initialize the hash to a 'random' value
    uint32_t hash = seed ^ len;

    // Mix 4 bytes at a time into the hash
    unsigned char const * data = (unsigned char const *)key;

    while(len >= 4)
    {
      uint32_t k = *(uint32_t*)data;

      k *= magic;
      k ^= k >> r;
      k *= magic;

      hash *= magic;
      hash ^= k;

      data += 4;
      len -= 4;
    }

    // Handle the last few bytes of the input array

    switch (len)
    {
      case 3: hash ^= data[2] << 16;
      case 2: hash ^= data[1] << 8;
      case 1: hash ^= data[0];
        hash *= magic;
        break;

      case 0:
        break;

      default:
        assert(0 && "This code should never be reached");
        break;
    };

    // Do a few final mixes of the hash to ensure the last few bytes are well-incorporated.
    hash ^= hash >> 13;
    hash *= magic;
    hash ^= hash >> 15;

    return hash;
  }

}