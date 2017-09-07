#include "saa/projection.hpp"

#include <cmath>

#include "saa/math.hpp"

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

Mat4f perspective_projection(const float fov_deg, const float aspect_ratio,
                             const float near, const float far) {
  const float fov_rad = static_cast<float>(degrees_to_radians(fov_deg));
  const float y_scale = 1 / std::tan(fov_rad / 2);
  const float x_scale = y_scale / aspect_ratio;

  Mat4f P = Mat4f::Zero();
  // clang-format off
  P <<
    x_scale, 0, 0, 0,
    0, y_scale, 0, 0,
    0, 0, -(far+near) / (far-near), -1,
    0, 0, -2*near*far / (far-near), 0;
  // clang-format on

  return P;
}

}  // namespace saa
