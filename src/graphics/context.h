
#pragma once

#include "foundation/array.h"
#include "graphics/context_types.h"
#include "graphics/image_types.h"

namespace three {
  namespace context {
    void uploadTexture(foundation::Stream & commandBuffer, TextureRef texture, Image & image);
  }
}