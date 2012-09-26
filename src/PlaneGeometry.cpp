
#include "PlaneGeometry.h"

namespace three {

  PlaneGeometry::PlaneGeometry(float width, float depth, int segmentsWidth, int segmentsDepth)
    : Geometry()
  {
    const float halfWidth = width * 0.5f;
    const float halfDepth = depth * 0.5f;
    const float segmentWidth = width / segmentsWidth;
    const float segementDepth = depth / segmentsDepth;

    for (int y = 0; y < segmentsDepth + 1; ++y)
      for (int x = 0; x < segmentsWidth + 1; ++x)
      {
        vertices.push_back(Vector3(x * segmentWidth - halfWidth, 0.0f, y * segementDepth - halfDepth));
        normals.push_back(Vector3(0, 1, 0));
        texCoord0.push_back(Vector2(x / (float)segmentsWidth, y / (float)segmentsDepth));
      }

    for (int y = 0; y < segmentsDepth; ++y)
      for (int x = 0; x < segmentsWidth; ++x)
      {
        uint16_t a = x + (segmentsWidth + 1) * y;
        uint16_t b = x + (segmentsWidth + 1) * (y + 1 );
        uint16_t c = (x + 1) + (segmentsWidth + 1) * (y + 1);
        uint16_t d = (x + 1) + (segmentsWidth + 1) * y;

        faces.push_back(Face(a, b, c));
        faces.push_back(Face(c, d, a));
      }

    computeBoundingSphere();
  }

}