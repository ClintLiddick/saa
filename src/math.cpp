#include "saa/math.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

namespace saa {

double degrees_to_radians(const double degrees) {
  return degrees * M_PI / 180.0;
}

}  // namespace
