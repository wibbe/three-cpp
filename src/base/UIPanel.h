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

#include "base/Object.h"
#include "base/Vector2.h"

namespace three {

  class Mesh;
  class Font;

  class UIPanel : public Object
  {
    public:
      static uint32_t Type;

    public:
      UIPanel(Font * font);

      uint32_t type() const { return UIPanel::Type; }

      bool begin(Vector2 const& mousePos, bool mouseDown, float mouseScroll);
      void end();

      void label(const char * str);
      void value(const char * str, ...);
      bool button(const char * str, bool enabled = true);
      bool checkbox(const char * str, bool checked, bool enabled = true);
      bool slider(const char * text, float * value, float valueMin, float valueMax, float valueInc, bool enabled = true);

      void indent();
      void unindent();
      void separator();
      void separatorLine();

    public:
      Vector2 size;
      Font * font;

      Mesh * _faceMesh;
      Mesh * _fontMesh;

      struct State
      {
        bool mouseDown;
        bool mousePressed;
        bool mouseReleased;
        Vector2 mousePos;
        Vector2 mouseDrag;
        float mouseScroll;

        float dragOrigin;
        float scroll;

        uint32_t active;
        uint32_t hot;
        uint32_t toBeHot;

        int32_t widgetId;

        bool isHot;
        bool isActive;
        bool wentActive;
        bool insidePanel;

        float widgetX, widgetY, widgetW;
      } _state;
  };

}

