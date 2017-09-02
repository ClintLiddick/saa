#pragma once

#include <Eigen/Dense>

namespace saa {

using Vec3f = Eigen::Vector3f;
using Vec4f = Eigen::Matrix<float, 4, 1>;

using Mat4f = Eigen::Matrix<float, 4, 4>;
using Mat3Xf = Eigen::Matrix<float, 3, Eigen::Dynamic>;

// TODO: trans mat = Eigen::Affine3f.data()

}  // namespace saa
