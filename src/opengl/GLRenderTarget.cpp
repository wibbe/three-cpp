
#include "GLRenderTarget.h"
#include "base/RenderTarget.h"

namespace three {

  GLRenderTarget::GLRenderTarget(RenderTarget * buffer)
    : BackendRenderTarget(buffer),
      frameBufferObject(0)
  {
    buffer->__renderBuffer = this;
  }

}