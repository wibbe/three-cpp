
#pragma once

#include "foundation/stream_types.h"
#include "foundation/memory_types.h"

namespace graphics {

  // -- Interface --
  enum Command
  {
    CMD_UPLOAD_TEXTURE,
    CMD_UPLOAD_BUFFER,
    CMD_UPLOAD_SHADER
  };


  typedef uint32_t TextureRef;
  typedef uint32_t BufferRef;
  typedef uint32_t ShaderRef;

  class Context
  {
    public:
      Context(foundation::Allocator & allocator);
      virtual ~Context();

      virtual TextureRef createTexture() = 0;
      virtual BufferRef createBuffer() = 0;
      virtual ShaderRef createShader() = 0;

      virtual void pushState() = 0;
      virtual void popState() = 0;

      virtual void executeBuffer(foundation::Stream const& buffer) = 0;
  };
}