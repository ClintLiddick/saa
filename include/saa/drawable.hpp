#pragma once

#include "saa/types.hpp"

namespace saa {

class Drawable {
public:
  virtual ~Drawable() {}

protected:
  friend class Window;

  virtual void initialize() = 0;
  virtual void draw(const Mat4f &clip_from_world) = 0;
  virtual void upload() = 0;
  virtual bool need_to_upload() { return false; }
};

}  // namespace saa
