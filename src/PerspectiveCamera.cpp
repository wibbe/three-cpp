
#include "PerspectiveCamera.h"

namespace three {

  PerspectiveCamera::PerspectiveCamera(float _fov, float _aspect, float _near, float _far)
    : Camera(),
      fov(_fov),
      aspect(_aspect),
      near(_near),
      far(_far)
  {
    updateProjectionMatrix();
  }

  void PerspectiveCamera::setLens(float focalLength, float frameHeight)
  {
    fov = 2.0f * std::atan(frameHeight / (focalLength * 2.0f)) * (180.0f / PI);
    updateProjectionMatrix();
  }

  void PerspectiveCamera::updateProjectionMatrix()
  {
    projectionMatrix.makePerspective(fov, aspect, near, far);
  }

}

