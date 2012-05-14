
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "RenderPlugin.h"
#include "Mesh.h"
#include "Geometry.h"
#include "GLObject.h"
#include "GLGeometry.h"

#include <stdio.h>
#include <stdlib.h>
#include <cassert>

#include <GL/glew.h>
#include <GL/glfw.h>

namespace three {

  // There can only exist one renderer at the time.
  Renderer * g_renderer = 0;

  Renderer::Renderer()
    : autoUpdateScene(true),
      autoUpdateObjects(true),
      autoClear(true),
      autoClearColor(true),
      autoClearDepth(true),
      autoClearStencil(false)
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

