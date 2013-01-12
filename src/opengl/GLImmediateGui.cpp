
#include "GLImmediateGui.h"

namespace three {

  GLImmediateGui::GLImmediateGui()
  {

  }

  void GLImmediateGui::init(Renderer * renderer)
  {

  }

  void GLImmediateGui::render(Scene * scene, Camera * camera, int viewportWidth, int viewportHeight)
  {

  }

  void GLImmediateGui::begin(int mouseX, int mouseY, bool mouseButton, int mouseScroll)
  {
    // Update input
    _mouseX = mouseX;
    _mouseY = mouseY;
    _mouseScroll = mouseScroll;
    _leftPressed = !_left && mouseButton;
    _leftReleased = _left && !mouseButton;
    _left = mouseButton;

    _hot = _toBeHot;
    _toBeHot = 0;

    _widgetX = 0;
    _widgetY = 0;
    _widgetWidth = 0;

    _wentActive = false;
    _isActive = false;
    _isHot = false;

    _areaId = 1;
    _widgetId = 1;
  }

  void GLImmediateGui::end()
  {
    clearInput();
  }

  void GLImmediateGui::clearInput()
  {
    _leftPressed = false;
    _leftReleased = false;
    _mouseScroll = 0;
  }

  void GLImmediateGui::clearActive()
  {
    _active = 0;
    clearInput();
  }

  bool GLImmediateGui::button(const char * text, bool enabled)
  {
    return false;
  }

}