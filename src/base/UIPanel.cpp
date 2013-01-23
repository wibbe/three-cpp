
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

  inline bool anyActive(UIPanel::State & state)
  {
    return state.active != 0;
  }

  inline bool isActive(UIPanel::State & state, uint32_t id)
  {
    return state.active == id;
  }

  inline bool isHot(UIPanel::State & state, uint32_t id)
  {
    return state.hot == id;
  }

  inline void clearActive(UIPanel::State & state)
  {
    state.active = 0;
    clearInput(state);
  }

  inline void setActive(UIPanel::State & state, uint32_t id)
  {
    state.active = id;
    state.wentActive = true;
  }

  inline void setHot(UIPanel::State & state, uint32_t id)
  {
     state.toBeHot = id;
  }

  inline bool inRect(UIPanel::State & state, float x, float y, float w, float h, bool checkScroll = true)
  {
    return (!checkScroll || state.insidePanel) && state.mousePos.x >= x && state.mousePos.x <= (x + w) && state.mousePos.y >= y && state.mousePos.y <= (y + h);
  }

  static bool buttonLogic(UIPanel::State & state, uint32_t id, bool over)
  {
    bool result = false;

    // process down
    if (!anyActive(state))
    {
      if (over)
        setHot(state, id);
      if (isHot(state, id) && state.mousePressed)
        setActive(state, id);
    }

    // if button is active, then react on left up
    if (isActive(state, id))
    {
      state.isActive = true;
      if (over)
        setHot(state, id);
      if (state.mouseReleased)
      {
        if (isHot(state, id))
          result = true;
        clearActive(state);
      }
    }

    if (isHot(state, id))
      state.isHot = true;

    return result;
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
    faceMat->doubleSided = true;

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

    _state.active = 0;
    _state.hot = 0;
    _state.toBeHot = 0;
  }

  bool UIPanel::begin(Vector2 const& mousePos, bool mouseDown, float mouseScroll)
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
    _state.insidePanel = inRect(_state, position.x, position.y, size.x, size.y, false);

    _state.widgetX = SCROLL_AREA_PADDING;
    _state.widgetY = 0;
    _state.widgetW = size.x - SCROLL_AREA_PADDING * 4;

    _state.widgetId = 1;

    // Change to rounded rect
    addRoundedRectangle(_faceMesh->geometry, Vector2(0, 0), size, 8.0f, Color(0, 0, 0, 0.75));

    return _state.insidePanel;
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

  bool UIPanel::button(const char * str, bool enabled)
  {
    _state.widgetId++;
    uint32_t id = _state.widgetId;

    float x = _state.widgetX;
    float y = _state.widgetY + BUTTON_HEIGHT;
    float w = _state.widgetW;
    float h = BUTTON_HEIGHT;
    _state.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

    bool over = enabled && inRect(_state, x, y, w, h);
    bool result = buttonLogic(_state, id, over);

    addRoundedRectangle(_faceMesh->geometry, Vector2(x, y), Vector2(w, h), BUTTON_HEIGHT * 0.5f - 1.0f, Color(0.5, 0.5, 0.5, isActive(_state, id) ? 0.75 : 0.35));

    if (enabled)
      font->buildTextGeometry(str, Vector2(x + BUTTON_HEIGHT * 0.5f, y + BUTTON_HEIGHT * 0.5f + TEXT_HEIGHT * 0.5f), _fontMesh->geometry, isHot(_state, id) ? Color(1, 0.75, 0, 1) : Color(1, 1, 1, 0.8));
    else
      font->buildTextGeometry(str, Vector2(x + BUTTON_HEIGHT * 0.5f, y + BUTTON_HEIGHT * 0.5f + TEXT_HEIGHT * 0.5f), _fontMesh->geometry, isHot(_state, id) ? Color(1, 0.75, 0, 1) : Color(0.5, 0.5, 0.5, 0.8));

    return result;
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

