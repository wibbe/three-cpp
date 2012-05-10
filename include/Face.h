
#pragma once

#include "Vector3.h"

namespace three {

  class Face
  {
    public:
      Face()
      { }

      Face(unsigned short _a, unsigned short _b, unsigned short _c,
           Vector3 const& _normal = Vector3(0.0f, 0.0f, 0.0f))
        : a(_a),
          b(_b),
          c(_c),
          materialIndex(-1),
          normal(_normal)
      { }

      Face(Face const& copy)
      {
        *this = copy;
      }

      void operator = (Face const& copy)
      {
        a = copy.a;
        b = copy.b;
        c = copy.c;
        normal = copy.normal;
        centroid = copy.centroid;

        //memcpy(vertexNormals, copy.vertexNormals, sizeof(Vector3) * 3);
        //memcpy(vertexTangents, copy.vertexTangents, sizeof(Vector3) * 3);
      }

    public:
      unsigned short a, b, c;
      int materialIndex;

      Vector3 normal;
      //Vector3 vertexNormals[3];
      //Vector3 vertexTangents[3];

      Vector3 centroid;
  };

}

