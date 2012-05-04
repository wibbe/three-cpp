
#include "Object.h"
#include "Math.h"

#include <algorithm>

namespace three {

  Object::Object()
    : parent(0),
      position(),
      rotation(),
      scale(1.0f, 1.0f, 1.0f),
      boundRadius(0.0f),
      boundRadiusScale(1.0f),
      visible(true),
      castShadow(false),
      receiveShadow(false),
      matrixWorldMatrixNeedsUpdate(true)
  {
  }

  Object::~Object()
  {
  }

  void Object::add(Object * object)
  {
    if (object->parent != 0)
      object->parent->remove(object);

    object->parent = this;
    children.push_back(object);
  }

  void Object::remove(Object * object)
  {
    children.erase(std::remove(children.begin(), children.end(), object), children.end());
  }

  void Object::updateMatrix()
  {
    matrix.setPosition(position);
    matrix.setRotationFromEuler(rotation);

    if (!equal(scale.x, 1.0f) || !equal(scale.y, 1.0f) || !equal(scale.z, 1.0f))
    {
      matrix.scale(scale);
      boundRadiusScale = scale.max();
    }
  }

}

