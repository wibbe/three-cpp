
#include "base/Camera.h"

namespace three {

  Camera::Camera()
    : Object(),
      cullObjects(true)
  {
  }

  void Camera::lookAt(Vector3 const& target)
  {
    matrix.lookAt(position, target, up);

    if (rotationAutoUpdate)
      rotation.getRotationFromMatrix(matrix);
  }

}
