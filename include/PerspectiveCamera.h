
#pragma once

#include "Camera.h"

namespace three {

  class PerspectiveCamera : public Camera
  {
    public:
      PerspectiveCamera(float fov = 50.0f, float aspect = 1.0f, float near = 0.1f, float far = 2000.0f);

      void setLens(float focalLength, float frameHeight = 24.0f);
      void updateProjectionMatrix();

    public:
      float fov;
      float aspect;
      float near;
      float far;
  };

}

