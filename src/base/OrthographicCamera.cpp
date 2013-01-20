
#include "base/OrthographicCamera.h"
#include "base/StringHash.h"

namespace three {

  unsigned int OrthographicCamera::Type = StringHash("OrthographicCamera").hash;

  OrthographicCamera::OrthographicCamera(float _left, float _right, float _top, float _bottom, float _near, float _far)
    : Camera(),
      left(_left),
      right(_right),
      top(_top),
      bottom(_bottom),
      near(_near),
      far(_far)
  {
    updateProjectionMatrix();
  }

  void OrthographicCamera::updateProjectionMatrix()
  {
    projectionMatrix.makeOrthographic(left, right, top, bottom, near, far);
  }

}

