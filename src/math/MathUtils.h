
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
  inline T abs(T const& a)
  {
    return a < 0 ? -a : a;
  }
  
  template <typename T>
  inline bool equal(T const& a, T const& b)
  {
    return abs(a - b) < EPSILON;
  }

  template <typename T>
  inline T clamp(T const& x, T const& a, T const& b)
  {
    return (x < a) ? a : ((x > b) ? b : x);
  }

  template <typename T>
  inline T max(T const& a, T const& b)
  {
    return a > b ? a : b;
  }
  
  template <typename T>
  inline T min(T const& a, T const& b)
  {
    return a < b ? a : b;
  }
}

