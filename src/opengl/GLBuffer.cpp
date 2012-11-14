
#include "GLBuffer.h"
#include "base/Buffer.h"

namespace three {

  GLBuffer::GLBuffer(Buffer * buffer)
    : RenderBuffer(buffer),
      frameBufferObject(0)
  {
    buffer->__renderBuffer = this;
  }

}