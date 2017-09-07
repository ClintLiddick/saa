#include "saa/camera.hpp"

#include <iostream> // TODO rm

namespace saa {

Mat4f look_at(const Vec3f &camera_pos, const Vec3f &target_pos,
              const Vec3f &up) {
  const Vec3f direction = (camera_pos - target_pos).normalized();
  const Vec3f right = up.cross(direction).normalized();
  Mat4f axes = Mat4f::Identity();
  axes.block<1,3>(0,0) = right.transpose();
  axes.block<1,3>(1,0) = up.transpose();
  axes.block<1,3>(2,0) = direction.transpose();
  Mat4f camera_pos_mat = Mat4f::Identity();
  camera_pos_mat.block<3,1>(0,3) = camera_pos;
    
  return axes * camera_pos_mat;
}

}  // namespace saa
