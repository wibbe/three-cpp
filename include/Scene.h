
#pragma once

#include "Object.h"

namespace three {
	
	class Scene : public Object
	{
    public:
      Scene();

      void __addObject(Object * object);
      void __removeObject(Object * object);
      
    public:
      bool matrixAutoUpdate;
      
    private:
      std::vector<Object *> objects;
      std::vector<Object *> lights;
      
      std::vector<Object *> objectsAdded;
      std::vector<Object *> objectsRemoved;
  };
}
