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

#include "Material.h"

namespace three {

  /*
    De olika materialen ska genereras från en ruby dsl. Både material och shaders måste kunna
    genereras lokalt för ett projekt, så det går att speca olika shaders/material utan att måsta
    kompilera om motorn.

    Det ska generaras medlems variabler i materialat som matchar de uniforms som finns i shadern.

    Materialet kanske bara ska spara shader namnen som ska användas, och specar man en egen shader i kod
    kommer den automatisk att läggas till projektet, och vara tillgänglig!?
  */

  // Forward declarations
  class Texture;

  class MeshBasicMaterial : public Material
  {
    public:
      static unsigned int Type;

    public:
      MeshBasicMaterial(Texture * map);

      unsigned int type() const { return MeshBasicMaterial::Type; }

    public:
      Texture * map;
  };

}
