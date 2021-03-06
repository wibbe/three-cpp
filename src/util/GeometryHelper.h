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

#include "base/Vector2.h"
#include "base/Vector3.h"
#include "base/Matrix4.h"
#include "base/Color.h"

namespace three {

  // Forward declaration
  class Geometry;

  void addQuad(Geometry * geom, Vector2 const& topLeft, Vector2 const& bottomRight, Vector2 const& uvTopLeft, Vector2 const& uvBottomRight, Color const& color);

  void addRectangle(Geometry * geom, Vector2 const& pos, Vector2 const& size, Color const& color);
  void addRoundedRectangle(Geometry * geom, Vector2 const& pos, Vector2 const& size, float radius, Color const& color);

  void addBox(Geometry * geom, Matrix4 const& transform, Vector3 const& size, Color const& color = Color(1, 1, 1));
  void addPlane(Geometry * geom, float width, float height, int segmentsWidth, int segmentsDepth, Color const& color = Color(1, 1, 1));

}

