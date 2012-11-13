
#include "base/Vector3.h"
#include "base/Matrix4.h"

#include <stdio.h>

namespace three {

  void Vector3::getRotationFromMatrix(Matrix4 const& mat, float scale)
  {
		float m11 = mat[0] / scale, m12 = mat[4] / scale, m13 = mat[8] / scale;
		float m21 = mat[1] / scale, m22 = mat[5] / scale, m23 = mat[9] / scale;
		float m33 = mat[10] / scale;

		y = std::asin(m13);

		float cosY = std::cos(y);

		if (abs(cosY) > 0.00001f)
		{
			x = std::atan2(- m23 / cosY, m33 / cosY);
			z = std::atan2(- m12 / cosY, m11 / cosY);
		}
		else
		{
			x = 0;
			z = std::atan2(m21, m22);
		}
	}

	std::string Vector3::str() const
	{
		char str[1024];
		snprintf(str, 1024, "[ %3.1f, %3.1f, %3.1f ]", x, y, z);
		return std::string(str);
	}

}

