
#include "GeometryHelper.h"
#include "base/Geometry.h"
#include "base/MathUtils.h"

#include <stdio.h>

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

  // Implementation borrows heavily from imguigl.cpp in

  static void addPolygons(Geometry * geom, Vector2 * vertices, int count, float feather, Color const& color)
  {
    uint16_t index = geom->vertices.size();

    // Add the first ring of vertices
    for (int i = 0; i < count; ++i)
    {
      geom->vertices.push_back(Vector3(vertices[i].x, vertices[i].y, 0));
      geom->colors.push_back(color);
    }

    // Create and add the second feathered ring of vertices
    Color transparent = color;
    transparent.a = 0.0;

    std::vector<Vector2> normals(count, Vector2(0, 0));

    // Calculate normals for the edges
    for (int i = 0, j = count - 1; i < count; j = i++)
    {
      const Vector2 & vec1 = vertices[j];
      const Vector2 & vec2 = vertices[i];

      normals[j] = (vec2 - vec1).normalized().perpendicular();
    }

    for (int i = 0, j = count - 1; i < count; j = i++)
    {
      const Vector2 & normal1 = normals[j];
      const Vector2 & normal2 = normals[i];

      Vector2 normal = ((normal1 + normal2) * 0.5f);
      float length = normal.lengthSqrt();
      if (length > 0.00001f)
      {
        float scale = 1.0f / length;
        if (scale > 10.0f)
          scale = 10.0f;

        normal.x *= scale;
        normal.y *= scale;
      }

      geom->vertices.push_back(Vector3(vertices[i].x + normal.x * feather, vertices[i].y + normal.y * feather, 0));
      geom->colors.push_back(transparent);
    }

    // Feathered edges
    for (int i = 0, j = count - 1; i < count; j = i++)
    {
      geom->faces.push_back(Face(index + count + j, index + j, index + i));
      geom->faces.push_back(Face(index + i, index + count + i, index + count + j));
    }

    // Fill polygon
    for (int i = 2; i < count; ++i)
      geom->faces.push_back(Face(index, index + i, index + i - 1));

    geom->verticesNeedUpdate = true;
    geom->colorsNeedUpdate = true;
    geom->elementsNeedUpdate = true;
  }

  void addRectangle(Geometry * geom, Vector2 const& pos, Vector2 const& size, Color const& color)
  {
    Vector2 vertices[] = {
      Vector2(pos.x, pos.y),
      Vector2(pos.x + size.x, pos.y),
      Vector2(pos.x + size.x, pos.y + size.y),
      Vector2(pos.x, pos.y + size.y)
    };

    addPolygons(geom, vertices, 4, 0.0f, color);
  }

  void addRoundedRectangle(Geometry * geom, Vector2 const& pos, Vector2 const& size, float radius, Color const& color)
  {
    const Vector2 p0(pos.x + size.x - radius, pos.y + radius); // top-right
    const Vector2 p1(pos.x + size.x - radius, pos.y + size.y - radius); // bottom-right
    const Vector2 p2(pos.x + radius, pos.y + size.y - radius); // bottom-left
    const Vector2 p3(pos.x + radius, pos.y + radius); // top-left
    const float step = M_PI / 16.0f;

    Vector2 vertices[] = {
      // top
      Vector2(pos.x + radius, pos.y),
      Vector2(pos.x + size.x - radius, pos.y),

      // top-right
      Vector2(p0.x + std::cos(step * 7) * radius, p0.y - std::sin(step * 7) * radius),
      Vector2(p0.x + std::cos(step * 6) * radius, p0.y - std::sin(step * 6) * radius),
      Vector2(p0.x + std::cos(step * 5) * radius, p0.y - std::sin(step * 5) * radius),
      Vector2(p0.x + std::cos(step * 4) * radius, p0.y - std::sin(step * 4) * radius),
      Vector2(p0.x + std::cos(step * 3) * radius, p0.y - std::sin(step * 3) * radius),
      Vector2(p0.x + std::cos(step * 2) * radius, p0.y - std::sin(step * 2) * radius),
      Vector2(p0.x + std::cos(step * 1) * radius, p0.y - std::sin(step * 1) * radius),

      // right
      Vector2(pos.x + size.x, pos.y + radius),
      Vector2(pos.x + size.x, pos.y + size.y - radius),

      // bottom-right
      Vector2(p1.x + std::cos(step * 31) * radius, p1.y - std::sin(step * 31) * radius),
      Vector2(p1.x + std::cos(step * 30) * radius, p1.y - std::sin(step * 30) * radius),
      Vector2(p1.x + std::cos(step * 29) * radius, p1.y - std::sin(step * 29) * radius),
      Vector2(p1.x + std::cos(step * 28) * radius, p1.y - std::sin(step * 28) * radius),
      Vector2(p1.x + std::cos(step * 27) * radius, p1.y - std::sin(step * 27) * radius),
      Vector2(p1.x + std::cos(step * 26) * radius, p1.y - std::sin(step * 26) * radius),
      Vector2(p1.x + std::cos(step * 25) * radius, p1.y - std::sin(step * 25) * radius),

      // bottom
      Vector2(pos.x + size.x - radius, pos.y + size.y),
      Vector2(pos.x + radius, pos.y + size.y),

      // bottom-left
      Vector2(p2.x + std::cos(step * 23) * radius, p2.y - std::sin(step * 23) * radius),
      Vector2(p2.x + std::cos(step * 22) * radius, p2.y - std::sin(step * 22) * radius),
      Vector2(p2.x + std::cos(step * 21) * radius, p2.y - std::sin(step * 21) * radius),
      Vector2(p2.x + std::cos(step * 20) * radius, p2.y - std::sin(step * 20) * radius),
      Vector2(p2.x + std::cos(step * 19) * radius, p2.y - std::sin(step * 19) * radius),
      Vector2(p2.x + std::cos(step * 18) * radius, p2.y - std::sin(step * 18) * radius),
      Vector2(p2.x + std::cos(step * 17) * radius, p2.y - std::sin(step * 17) * radius),

      // left
      Vector2(pos.x, pos.y + size.y - radius),
      Vector2(pos.x, pos.y + radius),

      // top-left
      Vector2(p3.x + std::cos(step * 15) * radius, p3.y - std::sin(step * 15) * radius),
      Vector2(p3.x + std::cos(step * 14) * radius, p3.y - std::sin(step * 14) * radius),
      Vector2(p3.x + std::cos(step * 13) * radius, p3.y - std::sin(step * 13) * radius),
      Vector2(p3.x + std::cos(step * 12) * radius, p3.y - std::sin(step * 12) * radius),
      Vector2(p3.x + std::cos(step * 11) * radius, p3.y - std::sin(step * 11) * radius),
      Vector2(p3.x + std::cos(step * 10) * radius, p3.y - std::sin(step * 10) * radius),
      Vector2(p3.x + std::cos(step *  9) * radius, p3.y - std::sin(step *  9) * radius)
    };

    addPolygons(geom, vertices, 36, 1.0f, color);
  }

}