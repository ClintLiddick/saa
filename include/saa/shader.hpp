#pragma once

#include <string>

#include "saa/gl.h"

namespace saa {

class Shader final {
public:
  ~Shader();

  Shader(const Shader&) = default;
  Shader& operator=(const Shader&) = default;
  Shader(Shader&&) = default;
  Shader& operator=(Shader&&) = default;

  void use();

private:
  friend class Window;

  Shader(const std::string& vert_shader_path,
         const std::string& frag_shader_path);

  GLint program_;
};

}  // namespace saa
