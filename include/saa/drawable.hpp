#pragma once

namespace saa {

class Drawable {
public:
  virtual ~Drawable() {}

protected:
  friend class Window;

  virtual void draw() = 0;
  virtual void upload() = 0;
  virtual bool need_to_upload() { return false; }
};

}  // namespace saa
