
#include "CubeGeometry.h"

namespace three {

  CubeGeometry::CubeGeometry(float width, float height, float depth)
    : Geometry()
  {
    // Top
    vertices.push_back(Vector3(-width, height, depth));
    vertices.push_back(Vector3(-width, height, -depth));
    vertices.push_back(Vector3(width, height, -depth));
    vertices.push_back(Vector3(width, height, depth));
    normals.push_back(Vector3(0, 1, 0));
    normals.push_back(Vector3(0, 1, 0));
    normals.push_back(Vector3(0, 1, 0));
    normals.push_back(Vector3(0, 1, 0));
    faces.push_back(Face(0, 1, 2));
    faces.push_back(Face(1, 2, 3));

  }

}

