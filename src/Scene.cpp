
#include "Scene.h"
#include "Camera.h"

#include <algorithm>

namespace three {
  
  Scene::Scene()
    : Object(),
      matrixAutoUpdate(false)
  {
  }

  void Scene::__addObject(Object * object)
  {
    // TODO: Check if it is a lighto
    if (!dynamic_cast<Camera *>(object))
    {
      objects.push_back(object);
      objectsAdded.push_back(object);

      objectsRemoved.erase(std::remove(objectsRemoved.begin(), objectsRemoved.end(), object), objectsRemoved.end());
    }

    for (std::vector<Object *>::iterator it = children.begin(), end = children.end(); it != end; ++it)
      __addObject(*it);
  }

  void Scene::__removeObject(Object * object)
  {
    if (!dynamic_cast<Camera *>(object))
    {
      std::vector<Object *>::iterator result = std::find(objectsRemoved.begin(), objectsRemoved.end(), object);
      if (result != objectsRemoved.end())
      {
        objects.erase(result);
        objectsRemoved.push_back(object);

        objectsAdded.erase(std::remove(objectsAdded.begin(), objectsAdded.end(), object), objectsAdded.end());
      }
    }

    for (std::vector<Object *>::iterator it = children.begin(), end = children.end(); it != end; ++it)
      __removeObject(*it);
  }
  
}
