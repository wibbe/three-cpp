
#include "GLContext.h"

#include "foundation/memory.h"
#include "foundation/atomic.h"
#include "foundation/array.h"

namespace graphics {
  using namespace foundation;

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

      void executeBuffer(Stream const& buffer)
      {
      }

    public:
      AtomicInt _textureRef;
      AtomicInt _bufferRef;
      AtomicInt _shaderRef;

      Array<State> _stateStack;
  };

  // -- Interface --

  Context * createGLContext(Allocator & allocator)
  {
    return MAKE_NEW(allocator, GLContext, allocator);
  }

}