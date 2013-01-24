
#include "base/Object.h"
#include "base/Scene.h"
#include "base/StringHash.h"

#include <algorithm>

namespace three {

  uint32_t Object::Type = StringHash("Object").hash;

  Object::Object()
    : parent(0),
      name(""),
      z(1.0f),
      up(0.0f, 1.0f, 0.0f),
      position(),
      rotation(),
      scale(1.0f, 1.0f, 1.0f),
      boundRadius(0.0f),
      boundRadiusScale(1.0f),
      visible(true),
      castShadow(false),
      receiveShadow(false),
      rotationAutoUpdate(true),
      matrixAutoUpdate(true),
      matrixWorldNeedsUpdate(true),
      __renderObject(0)
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

    Object * root = this;
    while (root->parent)
      root = root->parent;

    if (Scene * scene = dynamic_cast<Scene *>(root))
      scene->__addObject(object);
  }

  void Object::remove(Object * object)
  {
    children.erase(std::remove(children.begin(), children.end(), object), children.end());

    Object * root = this;
    while (root->parent)
      root = root->parent;

    if (Scene * scene = dynamic_cast<Scene *>(root))
      scene->__removeObject(object);
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
    matrix.identity();
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

    if (matrixWorldNeedsUpdate || force)
    {
      if (parent)
        matrixWorld = parent->matrixWorld * matrix;
      else
        matrixWorld = matrix;

      matrixWorldNeedsUpdate = false;
      force = true;
    }

    for (std::vector<Object *>::iterator it = children.begin(), end = children.end(); it != end; ++it)
      (*it)->updateWorldMatrix(force);
  }

  void Object::onUpdate(Camera * camera)
  {
  }

  void Object::onPreRender(Renderer * renderer)
  {
  }

  void Object::onPostRender(Renderer * renderer)
  {
  }

}

