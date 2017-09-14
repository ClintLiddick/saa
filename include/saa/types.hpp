#pragma once

#include <Eigen/Dense>

namespace saa {

using Vec3f = Eigen::Vector3f;
using Vec4f = Eigen::Matrix<float, 4, 1>;

using Mat4f = Eigen::Matrix<float, 4, 4>;

using Mat3Xf = Eigen::Matrix<float, 3, Eigen::Dynamic>;
using Mat4Xf = Eigen::Matrix<float, 4, Eigen::Dynamic>;
using Mat5Xf = Eigen::Matrix<float, 5, Eigen::Dynamic>;
using Mat6Xf = Eigen::Matrix<float, 6, Eigen::Dynamic>;

using Mat3Xu = Eigen::Matrix<uint, 3, Eigen::Dynamic>;

// TODO: trans mat = Eigen::Affine3f.data()

}  // namespace saa
