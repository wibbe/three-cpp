
#pragma once

namespace three {

  class Renderer
  {
    public:
      Renderer(int windowWidth, int windowHeight, bool fullscreen);
      ~Renderer();

      void setViewport(int x, int y, int width, int height);

      void clear(bool color, bool depth, bool stencil = false);

    private:
      void setDefaultGLState();

    private:
      int viewportX;
      int viewportY;
      int viewportWidth;
      int viewportHeight;
  };

}

