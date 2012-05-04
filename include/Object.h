
#pragma once

#include "Vector3.h"
#include "Matrix4.h"

#include <vector>

namespace three {

  /**
   * Base class for all objects in Three++
   */
  class Object
  {
    public:
      Object();
      virtual ~Object();

      void add(Object * object);
      void remove(Object * object);

      void updateMatrix();

    public:
      Object * parent;
      std::vector<Object *> children;

      Vector3 position;
      Vector3 rotation;
      Vector3 scale;

      Matrix4 matrix;
      Matrix4 matrixWorld;

      float boundRadius;

      bool visible;
      bool castShadow;
      bool receiveShadow;
  };

}

