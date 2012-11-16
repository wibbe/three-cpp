
#include "gl_context.h"

#include "foundation/memory.h"
#include "foundation/atomic.h"
#include "foundation/array.h"
#include "foundation/stream.h"

#include <stdio.h>

namespace three {
  using namespace foundation;

  class GLContext;

  struct State
  {
    int textureRef;
    int bufferRef;
    int shaderRef;
  };

  struct Texture
  {
    uint32_t id;
    uint16_t width;
    uint16_t height;
  };

  // -- Forward declarations --
  static void uploadTexture(Stream & s, GLContext & context);

  // -- OpenGL context --

  class GLContext : public Context
  {
    public:
      GLContext(Allocator & allocator)
        : Context(allocator),
          _stateStack(allocator)
      {
        atomic::set(_textureRef, 0);
        atomic::set(_bufferRef, 0);
        atomic::set(_shaderRef, 0);
      }

      virtual ~GLContext()
      {
      }

      TextureRef createTexture()
      {
        return atomic::inc(_textureRef);
      }

      BufferRef createBuffer()
      {
        return atomic::inc(_bufferRef);
      }

      ShaderRef createShader()
      {
        return atomic::inc(_shaderRef);
      }

      void pushState()
      {
        State state;
        state.textureRef = atomic::read(_textureRef);
        state.bufferRef = atomic::read(_bufferRef);
        state.shaderRef = atomic::read(_shaderRef);

        array::push_back(_stateStack, state);
      }

      void popState()
      {
        if (array::any(_stateStack))
        {
          State & state = array::back(_stateStack);

          atomic::set(_textureRef, state.textureRef);
          atomic::set(_bufferRef, state.bufferRef);
          atomic::set(_shaderRef, state.shaderRef);

          array::pop_back(_stateStack);
        }
      }

      void executeBuffer(Stream & buffer)
      {
        while (!stream::eof(buffer))
        {
          uint32_t command = command::NOP;
          buffer >> command;

          switch (command)
          {
            case command::UPLOAD_TEXTURE:
              uploadTexture(buffer, *this);
              break;

            default:
              fprintf(stderr, "Unknown command: %d\n", command);
              break;
          }
        }
      }

    public:
      AtomicInt _textureRef;
      AtomicInt _bufferRef;
      AtomicInt _shaderRef;

      Array<State> _stateStack;
  };

  // -- Command implementations --

  static void uploadTexture(Stream & s, GLContext & context)
  {
    TextureRef ref;
    uint16_t width, height;
    uint8_t format, type;
    s >> ref >> width >> height >> format >> type;
  }

  // -- Interface --

  Context * createGLContext(Allocator & allocator)
  {
    return MAKE_NEW(allocator, GLContext, allocator);
  }

}