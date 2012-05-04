
#include "Renderer.h"

#include <stdio.h>
#include <stdlib.h>

namespace three {

  // There can only exist one renderer at the time.
  static Renderer * g_renderer = 0;

  Renderer::Renderer(int windowWidth, int windowHeight, bool fullscreen)
  {
    if (!g_renderer)
    {
      g_renderer = this;
    }
    else
    {
      fprintf(stderr, "There can only be one allocated renderer at all times.");
      exit(1);
    }
  }

  Renderer::~Renderer()
  {
    if (g_renderer == this)
      g_renderer = 0;
  }


}
