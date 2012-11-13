
#include "loader/CTMLoader.h"
#include "base/Mesh.h"
#include "base/Geometry.h"

#include <openctm.h>
#include <iostream>

namespace three {

  Mesh * CTMLoader::loadMesh(const char * name)
  {
    CTMcontext context = ctmNewContext(CTM_IMPORT);

    Mesh * mesh = 0;

    ctmLoad(context, name);
    if (ctmGetError(context) == CTM_NONE)
    {
      const int vertexCount = ctmGetInteger(context, CTM_VERTEX_COUNT);
      const int triangleCount = ctmGetInteger(context, CTM_TRIANGLE_COUNT);
      const float * positions = ctmGetFloatArray(context, CTM_VERTICES);
      const float * normals = ctmGetFloatArray(context, CTM_NORMALS);
      const float * texCoords = ctmGetFloatArray(context, CTM_UV_MAP_1);
      const uint32_t * indices = ctmGetIntegerArray(context, CTM_INDICES);

      Geometry * geometry = new Geometry();

      for (int i = 0; i < (vertexCount * 3); i += 3)
        geometry->vertices.push_back(Vector3(positions[i + 0], positions[i + 1], positions[i + 2]));

      if (normals)
      {
        for (int i = 0; i < (vertexCount * 3); i += 3)
          geometry->normals.push_back(Vector3(normals[i + 0], normals[i + 1], normals[i + 2]));
      }

      if (texCoords)
      {
        for (int i = 0; i < (vertexCount * 2); i += 2)
          geometry->texCoord0.push_back(Vector2(texCoords[i + 0], texCoords[i + 1]));
      }

      for (int i = 0; i < (triangleCount * 3); i += 3)
        geometry->faces.push_back(Face(indices[i + 0], indices[i + 1], indices[i + 2]));

      mesh = new Mesh(geometry, 0);
    }
    else
    {
      std::cerr << "Error while loading mesh '" << name << "': " << ctmErrorString(ctmGetError(context)) << std::endl;
    }

    ctmFreeContext(context);
    return mesh;
  }

}