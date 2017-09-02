#include "saa/shader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <iostream> // TODO rm

namespace saa {

Shader::Shader(const std::string &vert_shader_path,
               const std::string &frag_shader_path) {
  std::cout << "enter" << std::endl;
  std::ifstream vert_file{vert_shader_path};
  std::cout << "ifstream" << std::endl;
  std::stringstream vss;
  vss << vert_file.rdbuf();
  std::cout << "read" << std::endl;
  const std::string vert_str = vss.str();
  std::cout << "str" << std::endl;
  const char *vert_c_str = vert_str.c_str();
  std::cout << "c str" << std::endl;

  std::cout << "frag" << std::endl;
  std::ifstream frag_file{frag_shader_path};
  std::cout << "frag ifstream" << std::endl;
  std::stringstream fss;
  fss << frag_file.rdbuf();
  std::cout << "frag read" << std::endl;
  const std::string frag_str = fss.str();
  std::cout << "frag str" << std::endl;
  const char *frag_c_str = frag_str.c_str();
  std::cout << "frag c str" << std::endl;

  GLint success;
  GLchar compile_log[512];

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  std::cout << "created" << std::endl;
  glShaderSource(vs, 1, &vert_c_str, NULL);
  std::cout << "set source" << std::endl;
  glCompileShader(vs);
  std::cout << "compiled" << std::endl;
  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
  std::cout << "status" << std::endl;
  if (!success) {
  std::cout << "!success" << std::endl;
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

  std::cout << "creating prog" << std::endl;
  program_ = glCreateProgram();
  std::cout << "prog created" << std::endl;
  glAttachShader(program_, vs);
  glAttachShader(program_, fs);
  std::cout << "prog attached" << std::endl;
  glLinkProgram(program_);
  std::cout << "prog linked" << std::endl;
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

Shader::~Shader() { glDeleteProgram(program_); }

void Shader::use() { glUseProgram(program_); }

}  // namespace saa
