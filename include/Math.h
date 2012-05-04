
#pragma once

#include <cmath>

namespace three {
  
  const double PI = 3.1415926535897932;
  const double EPSILON = 0.0000001;
  
  inline double degToRad(double angle)
  {
    return (PI / 180.0) * angle;
  }
  
  template <typename T>
  inline bool equal(T const& a, T const& b)
  {
    return std::abs(a - b) < EPSILON;
  }

  template <typename T>
  inline T clamp(T const& x, T const& a, T const& b)
  {
    return (x < a) ? a : ((x > b) ? b : x);
  }
  
}

