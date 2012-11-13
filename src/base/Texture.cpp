
#include "base/Texture.h"
#include <memory.h>

namespace three {

  Texture::Texture()
    : width(0),
      height(0),
      wrapS(ClampToEdgeWrapping),
      wrapT(ClampToEdgeWrapping),
      magFilter(LinearFilter),
      minFilter(LinearMipMapLinearFilter),
      format(RGBAFormat),
      imageDataType(UnsignedByteType),
      type(Texture2D),
      needsUpdate(false),
      generateMipmaps(true),
      __renderTexture(0)
  {
    memset(images, 0, sizeof(void *));
  }

}

