
#include "opengl/GLTexture.h"
#include "Texture.h"

namespace three {

  GLTexture::GLTexture(Texture * source)
    : RenderTexture(source),
      id(0),
      type(0)
  {
    source->__renderTexture = this;
  }

}