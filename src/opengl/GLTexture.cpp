
#include "opengl/GLTexture.h"
#include "base/Texture.h"

namespace three {

  GLTexture::GLTexture(Texture * source)
    : BackendTexture(source),
      id(0),
      type(0)
  {
    source->__renderTexture = this;
  }

}