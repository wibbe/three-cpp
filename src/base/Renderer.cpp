
#include "base/Renderer.h"
#include "base/Scene.h"
#include "base/Camera.h"
#include "base/RenderPlugin.h"
#include "base/Mesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <cassert>

namespace three {

  // There can only exist one renderer at the time.
  Renderer * g_renderer = 0;

  Renderer::Renderer()
    : autoUpdateScene(true),
      autoUpdateObjects(true),
      autoClear(true),
      autoClearColor(true),
      autoClearDepth(true),
      autoClearStencil(false),
      sortObjects(true),
      overrideMaterial(0)
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

  void Renderer::addPrePlugin(RenderPlugin * plugin)
  {
    plugin->init(this);
    renderPluginsPre.push_back(plugin);
  }

  void Renderer::addPostPlugin(RenderPlugin * plugin)
  {
    plugin->init(this);
    renderPluginsPost.push_back(plugin);
  }

  void Renderer::renderPlugins(std::vector<RenderPlugin *> & plugins, Scene * scene, Camera * camera)
  {
    for (std::vector<RenderPlugin *>::iterator it = plugins.begin(), end = plugins.end(); it != end; ++it)
    {
      resetCache();
      (*it)->render(scene, camera, 0, 0);
      resetCache();
    }
  }

}

