
#include "base/CubeGeometry.h"

namespace three {

  static Vector3 dirs[6] = {
    Vector3(1, 0, 0),
    Vector3(-1, 0, 0),
    Vector3(0, 1, 0),
    Vector3(0, -1, 0),
    Vector3(0, 0, 1),
    Vector3(0, 0, -1)
  };

  CubeGeometry::CubeGeometry(float width, float height, float depth)
    : Geometry()
  {
    for (int i = 0; i < 6; ++i)
    {
      Vector3 normal = dirs[i];

      Vector3 up = std::abs(dot(normal, Vector3(0, 1, 0))) > 0.9f ? Vector3(0, 0, 1) : Vector3(0, 1, 0);
      Vector3 side = cross(normal, up) * Vector3(width, height, depth) * 0.5f;
      up = up * Vector3(width, height, depth) * 0.5f;

      Vector3 pos = normal * Vector3(width, height, depth) * 0.5f;

      uint16_t indexStart = vertices.size();

      // Top
      vertices.push_back(pos + side + up);
      vertices.push_back(pos + side - up);
      vertices.push_back(pos - side - up);
      vertices.push_back(pos - side + up);
      normals.push_back(normal);
      normals.push_back(normal);
      normals.push_back(normal);
      normals.push_back(normal);
      colors.push_back(Color(1, 1, 1, 1));
      colors.push_back(Color(1, 1, 1, 1));
      colors.push_back(Color(1, 1, 1, 1));
      colors.push_back(Color(1, 1, 1, 1));
      texCoord0.push_back(Vector2(1, 1));
      texCoord0.push_back(Vector2(1, 0));
      texCoord0.push_back(Vector2(0, 0));
      texCoord0.push_back(Vector2(0, 1));
      faces.push_back(Face(indexStart + 0, indexStart + 1, indexStart + 2));
      faces.push_back(Face(indexStart + 2, indexStart + 3, indexStart + 0));
    }

    computeBoundingSphere();
  }

}

