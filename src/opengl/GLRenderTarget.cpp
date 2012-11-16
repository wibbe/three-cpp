
#include "GLRenderTarget.h"
#include "base/RenderTarget.h"

namespace three {

  GLRenderTarget::GLRenderTarget(RenderTarget * buffer)
    : BackendRenderTarget(buffer),
      id(0)
  {
    buffer->__renderTarget = this;
  }

}