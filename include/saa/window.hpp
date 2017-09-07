#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "saa/drawable.hpp"
#include "saa/gl.h"
#include "saa/shader.hpp"
#include "saa/types.hpp"

namespace saa {

class Window final {
public:
  Window(const int width, const int height, const std::string &title);
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
  Window(Window &&) = delete;

  // Initialize and open the window and context.
  void open();
  // Close the OpenGL context and close the window.
  void close();
  // Block and wait for window to exit.
  void spin();

  void set_background_color(const Vec4f &color);

  void add_drawable(std::unique_ptr<Drawable> drawable);

  // Create a shader program in the current context.
  Shader create_shader(const std::string &vert_shader_path,
                       const std::string &frag_shader_path);

private:
  void initialize();
  void spin_gl();

  void handle_key_event(const int key, const int scancode, const int action,
                        const int mods);

private:
  static constexpr std::chrono::milliseconds MS_PER_FRAME{16};

  const int width_;
  const int height_;
  const std::string title_;

  std::thread thread_;
  std::mutex mutex_;

  GLFWwindow *window_;
  bool initialized_;
  bool should_close_;

  Vec4f bg_color_;
  std::vector<std::unique_ptr<Drawable>> drawables_;
};

}  // namespace saa
