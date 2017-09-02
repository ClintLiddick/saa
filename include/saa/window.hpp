#pragma once

#include <mutex>
#include <string>
#include <thread>

#include "saa/gl.h"

namespace saa {

class Window final {
public:
  Window(const int width, const int height, const std::string &title);
  ~Window();

  // Initialize and open the window and context.
  void open();
  // Close the OpenGL context and close the window.
  void close();
  // Block and wait for window to exit.
  void spin();

private:
  void initialize();
  void spin_gl();

private:
  const int width_;
  const int height_;
  const std::string title_;

  std::thread thread_;
  std::mutex mutex_;
  GLFWwindow *window_;
  bool should_close_;
};

}  // namespace saa
