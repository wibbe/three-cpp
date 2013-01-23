/**
 * Copyright (c) 2012 Daniel Wiberg
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

#include <string>
#include "base/Vector2.h"

namespace three {

  struct Key
  {
    enum Values
    {
       Space        = 32,
       Esc          = 256 + 1,
       F1           = 256 + 2,
       F2           = 256 + 3,
       F3           = 256 + 4,
       F4           = 256 + 5,
       F5           = 256 + 6,
       F6           = 256 + 7,
       F7           = 256 + 8,
       F8           = 256 + 9,
       F9           = 256 + 10,
       F10          = 256 + 11,
       F11          = 256 + 12,
       F12          = 256 + 13,
       F13          = 256 + 14,
       F14          = 256 + 15,
       F15          = 256 + 16,
       F16          = 256 + 17,
       F17          = 256 + 18,
       F18          = 256 + 19,
       F19          = 256 + 20,
       F20          = 256 + 21,
       F21          = 256 + 22,
       F22          = 256 + 23,
       F23          = 256 + 24,
       F24          = 256 + 25,
       F25          = 256 + 26,
       Up           = 256 + 27,
       Down         = 256 + 28,
       Left         = 256 + 29,
       Right        = 256 + 30,
       Lshift       = 256 + 31,
       Rshift       = 256 + 32,
       Lctrl        = 256 + 33,
       Rctrl        = 256 + 34,
       Lalt         = 256 + 35,
       Ralt         = 256 + 36,
       Tab          = 256 + 37,
       Enter        = 256 + 38,
       Backspace    = 256 + 39,
       Insert       = 256 + 40,
       Del          = 256 + 41,
       Pageup       = 256 + 42,
       Pagedown     = 256 + 43,
       Home         = 256 + 44,
       End          = 256 + 45,
       Kp_0         = 256 + 46,
       Kp_1         = 256 + 47,
       Kp_2         = 256 + 48,
       Kp_3         = 256 + 49,
       Kp_4         = 256 + 50,
       Kp_5         = 256 + 51,
       Kp_6         = 256 + 52,
       Kp_7         = 256 + 53,
       Kp_8         = 256 + 54,
       Kp_9         = 256 + 55,
       Kp_divide    = 256 + 56,
       Kp_multiply  = 256 + 57,
       Kp_subtract  = 256 + 58,
       Kp_add       = 256 + 59,
       Kp_decimal   = 256 + 60,
       Kp_equal     = 256 + 61,
       Kp_enter     = 256 + 62,
       Kp_num_lock  = 256 + 63,
       Caps_lock    = 256 + 64,
       Scroll_lock  = 256 + 65,
       Pause        = 256 + 66,
       Lsuper       = 256 + 67,
       Rsuper       = 256 + 68,
       Menu         = 256 + 69
    };
  };

  struct Mouse
  {
    enum Values
    {
      Left = 0,
      Middle = 1,
      Right = 2
    };
  };

  /// Used to construct a window with a valid OpenGL context.
  /// Note that there can only exists one window at the time.
  class Window
  {
    public:
      Window(int width, int height);
      virtual ~Window();

      void run();
      bool step();

      void setTitle(std::string const& title);

      virtual void resize(int width, int height) { }
      virtual bool update(double dt) { return true; }
      virtual void paint() = 0;
      virtual void mouseMoved(int x, int y) { }
      virtual void mousePressed(int button) { }
      virtual void mouseReleased(int button) { }

      Vector2 getMousePosition() const;
      float getMouseScoll() const;

      double time() const { return m_totalTime; }

      bool isKeyDown(int key) const;
      bool isMouseDown(int button) const;

    private:
      double m_lastTimeStamp;
      double m_totalTime;
  };

}
