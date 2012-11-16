
#pragma once

#include "foundation/memory_types.h"
#include "graphics/image_types.h"

namespace three {

  namespace image {
    Image * load(foundation::Allocator & allocator, const char * filename);

    uint32_t dataSize(Image & image);
    uint32_t elementCount(Image & image);
    uint32_t typeSize(Image & image);
  }

}