#pragma once

#include "saa/types.hpp"

namespace saa {

Mat4f ortho_projection(const float left, const float right, const float bottom,
                       const float top, const float near, const float far);

Mat4f perspective_projection(const float fov_deg, const float aspect_ratio,
                             const float near, const float far);

}  // namespace saa
