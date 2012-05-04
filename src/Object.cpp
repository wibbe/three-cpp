
#include "Object.h"
#include "Math.h"

#include <algorithm>

namespace three {

  Object::Object()
    : parent(0),
      name(""),
      up(0.0f, 1.0f, 0.0f),
      position(),
      rotation(),
      scale(1.0f, 1.0f, 1.0f),
      boundRadius(0.0f),
      boundRadiusScale(1.0f),
      visible(true),
      castShadow(false),
      receiveShadow(false),
      matrixAutoUpdate(true),
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

  Object * Object::getChildByName(std::string const& name, bool recursive)
  {
    for (std::vector<Object *>::iterator it = children.begin(), end = children.end(); it != end; ++it)
    {
      Object * child = *it;

      if (child->name == name)
        return child;

      if (recursive)
      {
        child = child->getChildByName(name, recursive);

        if (child)
          return child;
      }
    }

    return 0;
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

  void Object::updateWorldMatrix(bool force)
  {
    if (matrixAutoUpdate)
      updateMatrix();

    if (matrixWorldMatrixNeedsUpdate || force)
    {
      if (parent)
        matrixWorld = parent->matrixWorld * matrix;
      else
        matrixWorld = matrix;

      matrixWorldMatrixNeedsUpdate = false;
      force = true;
    }

    for (std::vector<Object *>::iterator it = children.begin(), end = children.end(); it != end; ++it)
      (*it)->updateWorldMatrix(force);
  }

}

