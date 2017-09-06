#include "saa/projection.hpp"

namespace saa {

Mat4f ortho_projection(const float l, const float r, const float b,
                       const float t, const float n, const float f) {
  Mat4f P;
  // clang-format off
  P <<
    2/(r-l), 0, 0, -(r+l)/(r-l),
    0, 2/(t-b), 0, -(t+b)/(t-b),
    0, 0, -2/(f-n), -(f+n)/(f-n),
    0, 0, 0, 1;
  // clang-format on

  return P;
}

}  // namespace saa
