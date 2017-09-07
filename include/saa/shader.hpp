#pragma once

#include <string>

#include "saa/gl.h"
#include "saa/types.hpp"

namespace saa {

class Shader final {
public:
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;
  Shader(Shader&&) = default;
  Shader& operator=(Shader&&) = default;

  void use();

  void set_clip_from_local(const Mat4f& clip_from_local);
  void set_uniformf(const std::string& name, const float value);

private:
  friend class Window;

  Shader(const std::string& vert_shader_path,
         const std::string& frag_shader_path);

  GLint program_;
};

}  // namespace saa
