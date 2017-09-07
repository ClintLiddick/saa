#pragma once

#include "saa/drawable.hpp"
#include "saa/gl.h"
#include "saa/types.hpp"
#include "saa/window.hpp"

namespace saa {

class Points final : public Drawable {
public:
  explicit Points(Window &window, const float point_size = 1.0);
  Points(Window &window, const Mat3Xf &points, const float point_size = 1.0);
  virtual ~Points() = default;

  Points(const Points &) = delete;
  Points &operator=(const Points &) = delete;

  Points(Points &&) = default;
  Points &operator=(Points &&) = default;

  void set_points(const Mat3Xf &points);
  void add_points(const Mat3Xf &points);
  void set_point_size(const float point_size);

  void initialize() override;
  void draw(const Mat4f &clip_from_world) override;
  void upload() override;
  bool need_to_upload() override;

private:
  Shader shader_;
  Mat3Xf points_;
  float point_size_;
  bool points_changed_;
  GLuint vao_;
  GLuint vbo_;
};

}  // namespace saa
