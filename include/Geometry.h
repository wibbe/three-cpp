
#pragma once

#include "Vector3.h"
#include "Color.h"
#include "Face.h"

#include <vector>

namespace three {

  class Geometry
  {
    public:
      typedef std::vector<Vector3> VertexArray;
      typedef std::vector<Color> ColorArray;
      typedef std::vector<Face> FaceArray;

    public:
      Geometry();

      void computeCentroids();

    public:
      VertexArray vertices;
      ColorArray colors;

      FaceArray faces;

      bool hasTangents;
      bool dynamic;
  };

}

