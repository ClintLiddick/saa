#pragma once

#include "saa/types.hpp"

namespace saa {

Mat4f look_at(const Vec3f &camera_pos, const Vec3f &target_pos,
              const Vec3f &up);

}  // namespace saa
