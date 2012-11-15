
#pragma once

#include "foundation/collection_types.h"
#include "foundation/memory_types.h"

namespace graphics {

  // -- Interface --
  namespace command {
    enum Command
    {
      UPLOAD_TEXTURE,
      UPLOAD_BUFFER,
      UPLOAD_SHADER
    };

    typedef foundation::Array<unsigned char> Buffer;
  }

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

      virtual void executeBuffer(command::Buffer const& buffer) = 0;
  };
}