#include "saa/drawing/points.hpp"

#include "config.hpp"

namespace saa {

namespace {

constexpr char VERT_SHADER_PATH[] = SAA_SHADER_PATH "/points.vs.glsl";
constexpr char FRAG_SHADER_PATH[] = SAA_SHADER_PATH "/points.fs.glsl";

}  // namespace

Points::Points(Window &window) : Points{{}, window} {}

Points::Points(const Mat3Xf &points, Window &window)
    : shader_{window.create_shader(VERT_SHADER_PATH, FRAG_SHADER_PATH)}
    , points_{points}
    , points_changed_{false}
    , vao_{0}
    , vbo_{0} {}

void Points::initialize() {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
}

void Points::set_points(const Mat3Xf &points) {
  points_ = points;
  points_changed_ = true;
}
void Points::add_points(const Mat3Xf &points) {
  const auto old_cols = points_.cols();
  points_.conservativeResize(points_.rows(), old_cols + points.cols());
  points_.block(0, old_cols, points_.rows() - 1, points_.cols() - 1) = points;
  points_changed_ = true;
}

void Points::draw() {
  if (points_.size() == 0) { return; }
  shader_.use();
  glBindVertexArray(vao_);
  {  //
    glDrawArrays(GL_POINTS, 0, points_.cols());
  }  //
  glBindVertexArray(0);
}

void Points::upload() {
  if (points_.size() == 0) { return; }
  glBindVertexArray(vao_);
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, points_.size() * sizeof(float),
                 points_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
  }
  glBindVertexArray(0);

  points_changed_ = false;
}

bool Points::need_to_upload() { return points_changed_; }

}  // namespace saa
