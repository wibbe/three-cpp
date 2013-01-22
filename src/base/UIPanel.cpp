
#include "base/UIPanel.h"
#include "base/Mesh.h"
#include "base/Geometry.h"
#include "base/Font.h"
#include "material/DefaultMaterials.h"
#include "base/StringHash.h"
#include "util/GeometryHelper.h"

#include <cassert>

namespace three {

  uint32_t UIPanel::Type = StringHash("UIPanel").hash;

  static const int BUTTON_HEIGHT = 20;
  static const int SLIDER_HEIGHT = 20;
  static const int SLIDER_MARKER_WIDTH = 10;
  static const int CHECK_SIZE = 8;
  static const int DEFAULT_SPACING = 4;
  static const int TEXT_HEIGHT = 8;
  static const int SCROLL_AREA_PADDING = 6;
  static const int INDENT_SIZE = 16;
  static const int AREA_HEADER = 28;

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


  UIPanel::UIPanel(Font * font_)
    : Object(),
      size(250, 350),
      font(font_),
      _faceMesh(0),
      _fontMesh(0)
  {
    assert(font);

    MeshBasicMaterial * faceMat = new MeshBasicMaterial();
    faceMat->useVertexColor = true;
    faceMat->diffuse = Color(1, 1, 1);
    faceMat->depthTest = false;
    faceMat->depthWrite = false;
    faceMat->transparent = true;

    MeshBasicMaterial * fontMat = new MeshBasicMaterial();
    fontMat->useTextureMap = true;
    fontMat->useVertexColor = true;
    fontMat->map = font->texture;
    fontMat->diffuse = Color(1, 1, 1);
    fontMat->depthTest = false;
    fontMat->depthWrite = false;
    fontMat->transparent = true;

    _faceMesh = new Mesh(new Geometry(), faceMat);
    _faceMesh->geometry->dynamic = true;

    // Create text mesh, this is always slightly above the face mesh
    _fontMesh = new Mesh(new Geometry(), fontMat);
    _fontMesh->position.z = -0.1;
    _fontMesh->geometry->dynamic = true;

    add(_faceMesh);
    add(_fontMesh);
  }

  void UIPanel::begin(Vector2 const& mousePos, bool mouseDown, float mouseScroll)
  {
    _faceMesh->geometry->clear();
    _fontMesh->geometry->clear();

    // Update input with panel normalized mouse coords
    updateInput(_state, mousePos - Vector2(position.x, position.y), mouseDown, mouseScroll);

    _state.hot = _state.toBeHot;
    _state.toBeHot = 0;

    _state.wentActive = false;
    _state.isActive = false;
    _state.isHot = false;

    _state.widgetX = SCROLL_AREA_PADDING;
    _state.widgetY = 0;
    _state.widgetW = size.x - SCROLL_AREA_PADDING * 4;

    _state.areaId = 1;
    _state.widgetId = 1;

    // Change to rounded rect
    addRectangle(_faceMesh->geometry, Vector2(0, 0), Vector2(size.x, size.y), Color(0, 0, 0, 0.75));
  }

  void UIPanel::end()
  {
    clearInput(_state);
  }

  void UIPanel::label(const char * str)
  {
    assert(font);

    float x = _state.widgetX;
    float y = _state.widgetY + BUTTON_HEIGHT;
    _state.widgetY += BUTTON_HEIGHT;

    font->buildTextGeometry(str, Vector2(x, y), _fontMesh->geometry, Color(1, 1, 1));
  }

  void UIPanel::indent()
  {
    _state.widgetX += INDENT_SIZE;
    _state.widgetW -= INDENT_SIZE;
  }

  void UIPanel::unindent()
  {
    _state.widgetX -= INDENT_SIZE;
    _state.widgetW += INDENT_SIZE;
  }

  void UIPanel::separator()
  {
    _state.widgetY += DEFAULT_SPACING * 3;
  }

  void UIPanel::separatorLine()
  {
    int x = _state.widgetX;
    int y = _state.widgetY + DEFAULT_SPACING * 2;
    int w = _state.widgetW;
    int h = 1;
    _state.widgetY += DEFAULT_SPACING * 4;

    addRectangle(_faceMesh->geometry, Vector2(x, y), Vector2(w, h), Color(1, 1, 1, 0.12));
  }
}

