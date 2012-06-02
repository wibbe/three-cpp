
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "StringHash.h"

#include <algorithm>

namespace three {

  unsigned int Scene::Type = StringHash("Scene").hash;
  
  Scene::Scene()
    : Object(),
      matrixAutoUpdate(false)
  {
  }

  void Scene::__addObject(Object * object)
  {
    if (dynamic_cast<Light *>(object))
    {
      lights.push_back(object);
    }
    else if (!dynamic_cast<Camera *>(object))
    {
      objects.push_back(object);
      objectsAdded.push_back(object);

      objectsRemoved.erase(std::remove(objectsRemoved.begin(), objectsRemoved.end(), object), objectsRemoved.end());
    }

    for (std::vector<Object *>::iterator it = object->children.begin(), end = object->children.end(); it != end; ++it)
      __addObject(*it);
  }

  void Scene::__removeObject(Object * object)
  {
    if (dynamic_cast<Light *>(object))
    {
        lights.erase(std::remove(lights.begin(), lights.end(), object), lights.end());
    }
    else if (!dynamic_cast<Camera *>(object))
    {
      std::vector<Object *>::iterator result = std::find(objectsRemoved.begin(), objectsRemoved.end(), object);
      if (result != objectsRemoved.end())
      {
        objects.erase(result);
        objectsRemoved.push_back(object);

        objectsAdded.erase(std::remove(objectsAdded.begin(), objectsAdded.end(), object), objectsAdded.end());
      }
    }

    for (std::vector<Object *>::iterator it = object->children.begin(), end = object->children.end(); it != end; ++it)
      __removeObject(*it);
  }
  
}
