
#include "GeometryHelper.h"
#include "base/Geometry.h"

namespace three {

  void addQuad(Geometry * geom, Vector2 const& topLeft, Vector2 const& bottomRight, Vector2 const& uvTopLeft, Vector2 const& uvBottomRight, Color const& color)
  {
    uint16_t index = geom->vertices.size();

    float x0 = topLeft.x;
    float x1 = bottomRight.x;
    float y0 = topLeft.y;
    float y1 = bottomRight.y;
    float u0 = uvTopLeft.x;
    float u1 = uvBottomRight.x;
    float v0 = uvTopLeft.y;
    float v1 = uvBottomRight.y;

    geom->vertices.push_back(Vector3(x0, y0, 0));
    geom->vertices.push_back(Vector3(x0, y1, 0));
    geom->vertices.push_back(Vector3(x1, y1, 0));
    geom->vertices.push_back(Vector3(x1, y0, 0));

    geom->texCoord0.push_back(Vector2(u0, v0));
    geom->texCoord0.push_back(Vector2(u0, v1));
    geom->texCoord0.push_back(Vector2(u1, v1));
    geom->texCoord0.push_back(Vector2(u1, v0));

    geom->colors.push_back(color);
    geom->colors.push_back(color);
    geom->colors.push_back(color);
    geom->colors.push_back(color);

    geom->faces.push_back(Face(index + 0, index + 1, index + 2));
    geom->faces.push_back(Face(index + 2, index + 3, index + 0));

    geom->verticesNeedUpdate = true;
    geom->colorsNeedUpdate = true;
    geom->texCoord0NeedUpdate = true;
    geom->elementsNeedUpdate = true;
  }

}