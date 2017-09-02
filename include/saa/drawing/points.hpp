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

  void set_points(const Mat3Xf &points);
  void add_points(const Mat3Xf &points);

  void draw() override;
  void upload() override;
  bool need_to_upload() override;

private:
  static constexpr char VERT_SHADER_PATH[] = "";
  static constexpr char FRAG_SHADER_PATH[] = "";

  Shader shader_;
  Mat3Xf points_;
  bool points_changed_;
};

}  // namespace saa
