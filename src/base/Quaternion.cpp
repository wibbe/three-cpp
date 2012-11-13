
#include "base/Quaternion.h"

namespace three {
  
  Matrix4 Quaternion::toMatrix() const
  {
  	float x2 = x * x;
  	float y2 = y * y;
  	float z2 = z * z;
  	float xy = x * y;
  	float xz = x * z;
  	float yz = y * z;
  	float wx = w * x;
  	float wy = w * y;
  	float wz = w * z;

  	// This calculation would be a lot more complicated for non-unit length quaternions
  	// Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
  	//   OpenGL
  	return Matrix4(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
  			           2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
  			           2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
  }
  
}
