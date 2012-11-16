
#include "context.h"
#include "image.h"
#include "foundation/stream.h"

namespace three {

  using namespace foundation;

  Context::Context(foundation::Allocator & allocator)
  {
  }

  namespace context {
    void uploadTexture(foundation::Stream & commandBuffer, TextureRef texture, Image & image)
    {
      commandBuffer << (uint32_t)texture << image.width << image.height << image.type << image.format;
      stream::push(commandBuffer, image._data, image::dataSize(image));
    }
  }

}