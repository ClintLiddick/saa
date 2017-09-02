#include "saa/window.hpp"

#include <stdexcept>

namespace saa {

Window::Window(const int width, const int height, const std::string &title)
    : width_{width}, height_{height}, title_{title}, should_close_{false} {}

Window::~Window() { should_close_ = true; }

void Window::open() {
  thread_ = std::thread{[this]() {
    initialize();
    spin_gl();
  }};
}

void Window::close() {
  const std::lock_guard<std::mutex> lck{mutex_};
  should_close_ = true;
}

void Window::spin() { thread_.join(); }

void Window::initialize() {
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  if (!glfwInit()) { throw std::runtime_error{"Unable to initialize GLFW"}; }

  window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
  if (!window_) { throw std::runtime_error{"Unable to create window."}; }

  glfwMakeContextCurrent(window_);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void Window::spin_gl() {
  while (true) {
    const std::lock_guard<std::mutex> lck{mutex_};

    if (should_close_ || glfwWindowShouldClose(window_)) { break; }

    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

  glfwTerminate();
}

}  // namespace saa
