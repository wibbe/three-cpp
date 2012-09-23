
#include "Camera.h"

namespace three {

  Camera::Camera()
    : Object()
  {
  }

  void Camera::lookAt(Vector3 const& target)
  {
    matrix.lookAt(position, target, up);

    if (rotationAutoUpdate)
      rotation.getRotationFromMatrix(matrix);
  }

}
