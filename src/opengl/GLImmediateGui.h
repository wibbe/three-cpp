/**
 * Copyright (c) 2013 Daniel Wiberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include "base/RenderPlugin.h"

namespace three {

  class GLImmediateGui : public RenderPlugin
  {
    public:
      GLImmediateGui();

      void init(Renderer * renderer);
      void render(Scene * scene, Camera * camera, int viewportWidth, int viewportHeight);

      void begin(int mouseX, int mouseY, bool mouseButton, int mouseScroll);
      void end();

      bool button(const char * text, bool enabled = true);

    private:
      void clearInput();
      void clearActive();

    private:
      int _mouseX;
      int _mouseY;
      int _mouseScroll;
      int _widgetX;
      int _widgetY;
      int _widgetWidth;

      unsigned int _active;
      unsigned int _hot;
      unsigned int _toBeHot;

      unsigned int _areaId;
      unsigned int _widgetId;

      bool _wentActive;
      bool _isActive;
      bool _isHot;

      bool _left;
      bool _leftPressed;
      bool _leftReleased;
  };

}