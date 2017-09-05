#pragma once

#include "saa/drawable.hpp"
#include "saa/gl.h"
#include "saa/types.hpp"
#include "saa/window.hpp"

namespace saa {

class Points final : public Drawable {
public:
  explicit Points(Window &window);
  Points(const Mat3Xf &points, Window &window);
  virtual ~Points() = default;

  Points(const Points &) = delete;
  Points &operator=(const Points &) = delete;

  Points(Points &&) = default;
  Points &operator=(Points &&) = default;

  void set_points(const Mat3Xf &points);
  void add_points(const Mat3Xf &points);

  void initialize() override;
  void draw() override;
  void upload() override;
  bool need_to_upload() override;

private:
  Shader shader_;
  Mat3Xf points_;
  bool points_changed_;
  GLuint vao_;
  GLuint vbo_;
};

}  // namespace saa
