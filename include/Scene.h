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

#include "Object.h"

namespace three {
	
	class Scene : public Object
	{
    public:
      static uint32_t Type;

    public:
      Scene();

      uint32_t type() const { return Scene::Type; }

      void __addObject(Object * object);
      void __removeObject(Object * object);
      
    public:
      bool matrixAutoUpdate;
      
      std::vector<Object *> objects;
      std::vector<Object *> lights;
      
      std::vector<Object *> objectsAdded;
      std::vector<Object *> objectsRemoved;

      // Only for internal use by the renderer
      std::vector<RenderObject *> __renderObjects;
  };
}
