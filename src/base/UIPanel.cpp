
#include "base/UIPanel.h"
#include "base/Mesh.h"
#include "base/Geometry.h"
#include "material/DefaultMaterials.h"
#include "base/StringHash.h"

namespace three {

  uint32_t UIPanel::Type = StringHash("UIPanel").hash;

  static void updateInput(UIPanel::State & state, Vector2 const& mousePos, bool mouseDown, float mouseScroll)
  {
    state.mousePos = mousePos;
    state.mousePressed = !state.mouseDown && mouseDown;
    state.mouseReleased = state.mouseDown && !mouseDown;
    state.mouseDown = mouseDown;
    state.mouseScroll = mouseScroll;
  }

  static void clearInput(UIPanel::State & state)
  {
    state.mousePressed = false;
    state.mouseReleased = false;
    state.mouseScroll = 0;
  }


  UIPanel::UIPanel()
    : Object(),
      size(200, 400),
      _faceMesh(0),
      _fontMesh(0)
  {
    MeshBasicMaterial * faceMat = new MeshBasicMaterial();
    faceMat->useTextureMap = false;
    faceMat->useLightMap = false;
    faceMat->useEnvMap = false;
    faceMat->doubleSided = true;
    faceMat->useVertexColor = true;
    faceMat->gammaCorrection = false;
    faceMat->useReflectivity = false;

    _faceMesh = new Mesh(new Geometry(), faceMat);
    add(_faceMesh);
  }

  void UIPanel::begin(Vector2 const& mousePos, bool mouseDown, float mouseScroll)
  {
    updateInput(_state, mousePos, mouseDown, mouseScroll);

    _state.hot = _state.toBeHot;
    _state.toBeHot = 0;

    _state.wentActive = false;
    _state.isActive = false;
    _state.isHot = false;

    _state.widgetX = 0;
    _state.widgetY = 0;
    _state.widgetW = 0;

    _state.areaId = 1;
    _state.widgetId = 1;
  }

  void UIPanel::end()
  {
    clearInput(_state);
  }

}

