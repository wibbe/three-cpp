
#include "base/Vector2.h"

#include <stdlib.h>
#include <stdint.h>
#include <vector>

namespace three {

  static const int EMPTY_CELL = -1;

  inline float randFloat()
  {
    return rand() / (float)RAND_MAX;
  }

  static uint32_t pointToGridIndex(Vector2 const& point, float cellSize, uint32_t width)
  {
    const int x = (int)(point.x / cellSize);
    const int y = (int)(point.y / cellSize);
    return (y * width) + x;
  }

  // Returns a random element in the supplied list
  static Vector2 randomPoint(std::vector<Vector2> & list)
  {
    const uint32_t index = rand() % list.size();
    const Vector2 point = list[index];

    // Move last element to the position of the removed element
    if (index != list.size() - 1)
      list[index] = list[list.size() - 1];

    list.pop_back();
    return point;
  }

  static bool insideRect(Vector2 const& point, float width, float height)
  {
    return point.x >= 0.0f && point.x <= width &&
           point.y >= 0.0f && point.y <= height;
  }

  // Returns true if a point lies in the neighbourhood of the supplied point
  static bool inNeighbourhood(Vector2 const& point, std::vector<Vector2> const& points, float cellSize, float minDistance, int32_t * grid, int32_t gridWidth, int32_t gridHeight)
  {
    const int32_t pointX = (int)(point.x / cellSize);
    const int32_t pointY = (int)(point.y / cellSize);
    const float minDistSquared = minDistance * minDistance;

    for (int x = -2; x < 3; ++x)
      for (int y = -2; y < 3; ++y)
      {
        const uint32_t index = (clamp(pointY + y, 0, gridHeight - 1) * gridWidth) + clamp(pointX + x, 0, gridWidth - 1);
        const int32_t cell = grid[index];

        if (cell != EMPTY_CELL && (point - points[cell]).lengthSqrt() < minDistSquared)
          return true;
      }

    return false;
  }

  static Vector2 generateRandomPointAround(Vector2 const& point, float minDistance)
  {
    const float r1 = randFloat();
    const float r2 = randFloat();

    // Random radius between minDistance and minDistance * 2
    const float radius = minDistance * (r1 + 1.0f);
    const float angle = 2.0f * M_PI * r2;

    return Vector2(point.x + radius * std::cos(angle), point.y + radius * std::sin(angle));
  }

  void generatePoissonSet(std::vector<Vector2> & result, float width, float height, float minDistance, uint32_t seed, int newPointCount)
  {
    const float cellSize = minDistance / std::sqrt(2.0);
    const uint32_t gridWidth = (int)(width / cellSize) + 1;
    const uint32_t gridHeight = (int)(height / cellSize) + 1;

    // Grid containing indices into the result list, -1 indicate an empty cell
    int32_t * grid = new int32_t[gridWidth * gridHeight];

    srand(seed);

    // Points left to process
    std::vector<Vector2> processList;

    for (uint32_t i = 0, count = gridWidth * gridHeight; i < count; ++i)
      grid[i] = EMPTY_CELL;

    { // Generate the first point
      Vector2 point(randFloat() * width, randFloat() * height);

      result.push_back(point);
      processList.push_back(point);
      grid[pointToGridIndex(point, cellSize, gridWidth)] = 0;
    }

    while (!processList.empty())
    {
      Vector2 point = randomPoint(processList);

      for (int i = 0; i < newPointCount; ++i)
      {
        Vector2 newPoint = generateRandomPointAround(point, minDistance);

        if (insideRect(newPoint, width, height) &&
            !inNeighbourhood(newPoint, result, cellSize, minDistance, grid, gridWidth, gridHeight))
        {
          result.push_back(newPoint);
          processList.push_back(newPoint);
          grid[pointToGridIndex(newPoint, cellSize, gridWidth)] = result.size() - 1;
        }
      }
    }

    delete[] grid;
  }

}