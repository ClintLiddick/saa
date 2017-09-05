#include "saa/shader.hpp"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace saa {

Shader::Shader(const std::string &vert_shader_path,
               const std::string &frag_shader_path) {
  std::ifstream vert_file{vert_shader_path};
  std::stringstream vss;
  vss << vert_file.rdbuf();
  const std::string vert_str = vss.str();
  const char *vert_c_str = vert_str.c_str();

  std::ifstream frag_file{frag_shader_path};
  std::stringstream fss;
  fss << frag_file.rdbuf();
  const std::string frag_str = fss.str();
  const char *frag_c_str = frag_str.c_str();

  GLint success;
  GLchar compile_log[512];

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vert_c_str, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vs, 512, NULL, compile_log);
    std::ostringstream ss;
    ss << "Error compiling vertex shader: " << compile_log;
    throw std::runtime_error{ss.str()};
  }

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &frag_c_str, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fs, 512, NULL, compile_log);
    std::ostringstream ss;
    ss << "Error compiling fragment shader: " << compile_log;
    throw std::runtime_error{ss.str()};
  }

  program_ = glCreateProgram();
  glAttachShader(program_, vs);
  glAttachShader(program_, fs);
  glLinkProgram(program_);
  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_, 512, NULL, compile_log);
    std::ostringstream ss;
    ss << "Error linking shader program: " << compile_log;
    throw std::runtime_error{ss.str()};
  }
  glDeleteShader(vs);
  glDeleteShader(fs);
}

void Shader::use() { glUseProgram(program_); }

}  // namespace saa
