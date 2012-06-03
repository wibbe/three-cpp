
#include "Texture.h"

namespace three {

  Texture::Texture()
    : image(0),
      width(0),
      height(0),
      wrapS(ClampToEdgeWrapping),
      wrapT(ClampToEdgeWrapping),
      magFilter(LinearFilter),
      minFilter(LinearMipMapLinearFilter),
      format(RGBAFormat),
      type(UnsignedByteType),
      needsUpdate(false),
      generateMipmaps(true)
  {
  }

}

