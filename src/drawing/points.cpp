#include "saa/drawing/points.hpp"

namespace saa {

constexpr char Points::VERT_SHADER_PATH[];
constexpr char Points::FRAG_SHADER_PATH[];

Points::Points(Window &window)
    : shader_{window.create_shader(VERT_SHADER_PATH, FRAG_SHADER_PATH)} {}

Points::Points(const Mat3Xf &points, Window &window)
    : shader_{window.create_shader(VERT_SHADER_PATH, FRAG_SHADER_PATH)}
    , points_{points}
    , points_changed_{true} {}

void Points::set_points(const Mat3Xf &points) {
  points_ = points;
  points_changed_ = true;
}
void Points::add_points(const Mat3Xf &points) {
  const auto old_cols = points_.cols();
  points_.conservativeResize(points_.rows(), old_cols + points.cols());
  points_.block(0, old_cols, points_.rows() - 1, points_.cols() - 1) = points;
}

void Points::draw() {}

void Points::upload() { points_changed_ = false; }

bool Points::need_to_upload() { return points_changed_; }

}  // namespace saa
