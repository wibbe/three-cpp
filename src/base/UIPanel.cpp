
#include "base/UIPanel.h"
#include "base/Mesh.h"
#include "base/Geometry.h"
#include "base/Font.h"
#include "material/DefaultMaterials.h"
#include "base/StringHash.h"
#include "base/MathUtils.h"
#include "base/Renderer.h"
#include "util/GeometryHelper.h"

#include <cassert>
#include <stdio.h>

namespace three {

  uint32_t UIPanel::Type = StringHash("UIPanel").hash;

  static const int BUTTON_HEIGHT = 20;
  static const int SLIDER_HEIGHT = 20;
  static const int SLIDER_MARKER_WIDTH = 10;
  static const int CHECK_SIZE = 8;
  static const int DEFAULT_SPACING = 4;
  static const int TEXT_HEIGHT = 8;
  static const int SCROLL_AREA_PADDING = 6;
  static const int SCROLL_BAR_WIDTH = 3;
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

  // -- UIPanelMesh --

  class UIPanelMesh : public Mesh
  {
    public:
      UIPanelMesh(Geometry * geometry, Material * material, UIPanel * panel_)
        : Mesh(geometry, material),
          panel(panel_)
      { }

      void onPreRender(Renderer * renderer)
      {
        renderer->setScissor(true, panel->position.x, panel->position.y, panel->size.x, panel->size.y);
      }

      void onPostRender(Renderer * renderer)
      {
        renderer->setScissor(false);
      }

      UIPanel * panel;
  };

  // -- UIPanel --

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

    _faceMesh = new UIPanelMesh(new Geometry(), faceMat, this);
    _faceMesh->geometry->dynamic = true;

    // Create text mesh, this is always slightly above the face mesh
    _fontMesh = new UIPanelMesh(new Geometry(), fontMat, this);
    _fontMesh->position.z = -0.1;
    _fontMesh->geometry->dynamic = true;

    add(_faceMesh);
    add(_fontMesh);

    _state.active = 0;
    _state.hot = 0;
    _state.toBeHot = 0;
    _state.scroll = 0;
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
    _state.widgetY = SCROLL_AREA_PADDING - _state.scroll;
    _state.widgetW = size.x - SCROLL_AREA_PADDING * 3;

    _state.widgetId = 1;

    addRoundedRectangle(_faceMesh->geometry, Vector2(0, 0), size, 8.0f, Color(0, 0, 0, 0.75));
    return _state.insidePanel;
  }

  void UIPanel::end()
  {
    _state.widgetY -= SCROLL_AREA_PADDING;
    float height = size.y - SCROLL_AREA_PADDING * 2;
    float diff = _state.widgetY + _state.scroll - height;
    float factor = height / (_state.widgetY + _state.scroll);

    if (factor < 1.0f)
    {
      float x = size.x - SCROLL_AREA_PADDING - SCROLL_BAR_WIDTH * 0.5f;
      float h = factor * height;
      float y = SCROLL_AREA_PADDING + (height - h) * (_state.scroll / diff);

      addRoundedRectangle(_faceMesh->geometry, Vector2(x, y), Vector2(SCROLL_BAR_WIDTH, h), SCROLL_BAR_WIDTH * 0.5f, Color(1, 1, 1, 0.25));

      // Update scroll
      if (_state.insidePanel)
      {
        _state.scroll -= _state.mouseScroll * 20.0f;
        _state.scroll = max(0.0f, min(diff, _state.scroll));
      }
    }

    clearInput(_state);
  }

  void UIPanel::label(const char * str)
  {
    assert(font);

    float x = _state.widgetX;
    float y = _state.widgetY + BUTTON_HEIGHT * 0.5f;
    _state.widgetY += BUTTON_HEIGHT;

    font->buildTextGeometry(str, Vector2(x, y), _fontMesh->geometry, Color(1, 1, 1));
  }

  bool UIPanel::button(const char * str, bool enabled)
  {
    _state.widgetId++;
    uint32_t id = _state.widgetId;

    const float x = _state.widgetX;
    const float y = _state.widgetY;
    const float w = _state.widgetW;
    const float h = BUTTON_HEIGHT;
    _state.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

    bool over = enabled && inRect(_state, x, y, w, h);
    bool result = buttonLogic(_state, id, over);

    addRoundedRectangle(_faceMesh->geometry, Vector2(x, y), Vector2(w, h), BUTTON_HEIGHT * 0.5f - 1.0f, Color(0.5, 0.5, 0.5, isActive(_state, id) ? 0.75 : 0.35));

    const float textLength = font->textLength(str);
    const float textX = x + BUTTON_HEIGHT * 0.5f + (w - BUTTON_HEIGHT) * 0.5f - textLength * 0.5f;

    if (enabled)
      font->buildTextGeometry(str, Vector2(textX, y + BUTTON_HEIGHT * 0.5f + TEXT_HEIGHT * 0.5f), _fontMesh->geometry, isHot(_state, id) ? Color(1, 0.75, 0, 1) : Color(1, 1, 1, 0.8));
    else
      font->buildTextGeometry(str, Vector2(textX, y + BUTTON_HEIGHT * 0.5f + TEXT_HEIGHT * 0.5f), _fontMesh->geometry, isHot(_state, id) ? Color(1, 0.75, 0, 1) : Color(0.5, 0.5, 0.5, 0.8));

    return result;
  }

  bool UIPanel::checkbox(const char * str, bool checked, bool enabled)
  {
    _state.widgetId++;
    uint32_t id = _state.widgetId;

    float x = _state.widgetX;
    float y = _state.widgetY; // + BUTTON_HEIGHT;
    float w = _state.widgetW;
    float h = BUTTON_HEIGHT;
    _state.widgetY += BUTTON_HEIGHT + DEFAULT_SPACING;

    bool over = enabled && inRect(_state, x, y, w, h);
    bool result = buttonLogic(_state, id, over);

    const float cx = x + BUTTON_HEIGHT * 0.5f - CHECK_SIZE * 0.5f;
    const float cy = y + BUTTON_HEIGHT * 0.5f - CHECK_SIZE * 0.5f;

    addRoundedRectangle(_faceMesh->geometry, Vector2(cx - 3.0f, cy - 3.0f), Vector2(CHECK_SIZE + 6.0f, CHECK_SIZE + 6.0f), 4.0f, Color(0.5, 0.5, 0.5, isActive(_state, id) ? 0.75 : 0.35));

    if (checked)
    {
      if (enabled)
        addRoundedRectangle(_faceMesh->geometry, Vector2(cx, cy), Vector2(CHECK_SIZE, CHECK_SIZE), CHECK_SIZE * 0.5f - 1.0f, Color(1, 1, 1, isActive(_state, id) ? 1 : 0.8));
      else
        addRoundedRectangle(_faceMesh->geometry, Vector2(cx, cy), Vector2(CHECK_SIZE, CHECK_SIZE), CHECK_SIZE * 0.5f - 1.0f, Color(0.5, 0.5, 0.5, 0.8));
    }

    if (enabled)
      font->buildTextGeometry(str, Vector2(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT * 0.5f + TEXT_HEIGHT * 0.5f), _fontMesh->geometry, isHot(_state, id) ? Color(1, 0.75, 0, 1) : Color(1, 1, 1, 0.8));
    else
      font->buildTextGeometry(str, Vector2(x + BUTTON_HEIGHT, y + BUTTON_HEIGHT * 0.5f + TEXT_HEIGHT * 0.5f), _fontMesh->geometry, Color(0.5, 0.5, 0.5, 0.8));

    return result;
  }

  bool UIPanel::slider(const char * text, float * value, float valueMin, float valueMax, float valueInc, bool enabled)
  {
    _state.widgetId++;
    uint32_t id = _state.widgetId;

    const float x = _state.widgetX;
    const float y = _state.widgetY;
    const float w = _state.widgetW;
    const float h = SLIDER_HEIGHT;
    _state.widgetY += SLIDER_HEIGHT + DEFAULT_SPACING;

    addRoundedRectangle(_faceMesh->geometry, Vector2(x, y), Vector2(w, h), 4.0f, Color(0, 0, 0, 0.5));

    const float range = w - SLIDER_MARKER_WIDTH;

    float u = (*value - valueMin) / (valueMax - valueMin);
    if (u < 0.0f) u = 0.0f;
    if (u > 1.0f) u = 1.0f;
    float m = u * range;

    bool over = enabled && inRect(_state, x + m, y, SLIDER_MARKER_WIDTH, SLIDER_HEIGHT);
    bool result = buttonLogic(_state, id, over);
    bool valueChanged = false;

    if (isActive(_state, id))
    {
      if (_state.wentActive)
      {
        _state.mouseDrag.x = _state.mousePos.x;
        _state.dragOrigin = u;
      }

      if (_state.mouseDrag.x != _state.mousePos.x)
      {
        u = _state.dragOrigin + (float)(_state.mousePos.x - _state.mouseDrag.x) / (float)range;
        if (u < 0) u = 0;
        if (u > 1) u = 1;
        *value = valueMin + u * (valueMax - valueMin);
        *value = floorf(*value / valueInc + 0.5f) * valueInc; // Snap to vinc
        m = u * range;
        valueChanged = true;
      }
    }

    if (isActive(_state, id))
      addRoundedRectangle(_faceMesh->geometry, Vector2(x + m, y), Vector2(SLIDER_MARKER_WIDTH, SLIDER_HEIGHT), 4.0f, Color(1, 1, 1, 1));
    else
      addRoundedRectangle(_faceMesh->geometry, Vector2(x + m, y), Vector2(SLIDER_MARKER_WIDTH, SLIDER_HEIGHT), 4.0f, isHot(_state, id) ? Color(1, 0.75, 0, 1) : Color(1, 1, 1, 0.25));

    char valueStr[16];
    sprintf(valueStr, "%d%%", (int)std::ceil((*value - valueMin) / (valueMax - valueMin) * 100.0));
    const float valueLength = font->textLength(valueStr);

    Color fontColor = isHot(_state, id) ? Color(1, 0.75, 0, 1) : Color(1, 1, 1, 0.8);

    if (!enabled)
      fontColor = Color(0.5, 0.5, 0.5, 0.8);

    font->buildTextGeometry(text, Vector2(x + SLIDER_HEIGHT * 0.5f, y + SLIDER_HEIGHT * 0.5f + TEXT_HEIGHT * 0.5f), _fontMesh->geometry, fontColor);
    font->buildTextGeometry(valueStr, Vector2(x + w - valueLength - SLIDER_HEIGHT * 0.5f, y + SLIDER_HEIGHT * 0.5f + TEXT_HEIGHT * 0.5f), _fontMesh->geometry, fontColor);

/*
    if (enabled)
    {
      addGfxCmdText(x+SLIDER_HEIGHT/2, y+SLIDER_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, isHot(id) ? imguiRGBA(255,196,0,255) : imguiRGBA(255,255,255,200));
      addGfxCmdText(x+w-SLIDER_HEIGHT/2, y+SLIDER_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_RIGHT, msg, isHot(id) ? imguiRGBA(255,196,0,255) : imguiRGBA(255,255,255,200));
    }
    else
    {
      addGfxCmdText(x+SLIDER_HEIGHT/2, y+SLIDER_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_LEFT, text, imguiRGBA(128,128,128,200));
      addGfxCmdText(x+w-SLIDER_HEIGHT/2, y+SLIDER_HEIGHT/2-TEXT_HEIGHT/2, IMGUI_ALIGN_RIGHT, msg, imguiRGBA(128,128,128,200));
    }
*/
    return result || valueChanged;
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
    int y = _state.widgetY; // + DEFAULT_SPACING * 0.5f;
    int w = _state.widgetW;
    int h = 1;
    _state.widgetY += DEFAULT_SPACING * 2;

    addRectangle(_faceMesh->geometry, Vector2(x, y), Vector2(w, h), Color(1, 1, 1, 0.12));
  }
}

