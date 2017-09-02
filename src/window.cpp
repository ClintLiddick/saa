#include "saa/window.hpp"
#include <stdexcept>

namespace saa {

//
// Public
//

Window::Window(const int width, const int height, const std::string &title)
    : width_{width}
    , height_{height}
    , title_{title}
    , should_close_{false}
    , bg_color_{0.0, 0.0, 0.0, 1.0} {}

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

void Window::set_background_color(const Vec4f &color) {
  const std::lock_guard<std::mutex> lck{mutex_};
  bg_color_ = color;
}

void Window::add_drawable(std::unique_ptr<Drawable> drawable) {
  drawables_.push_back(std::move(drawable));
}

Shader Window::create_shader(const std::string &vert_shader_path,
                             const std::string &frag_shader_path) {
  const std::lock_guard<std::mutex> lck{mutex_};
  return Shader{vert_shader_path, frag_shader_path};
}

//
// Private
//

void Window::initialize() {
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  if (!glfwInit()) { throw std::runtime_error{"Unable to initialize GLFW"}; }

  window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
  if (!window_) { throw std::runtime_error{"Unable to create window."}; }

  glfwMakeContextCurrent(window_);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error{"Unable to load OpenGL context."};
  }
}

void Window::spin_gl() {
  while (true) {
    const std::lock_guard<std::mutex> lck{mutex_};

    if (should_close_ || glfwWindowShouldClose(window_)) { break; }
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(bg_color_[0], bg_color_[1], bg_color_[2], bg_color_[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &d : drawables_) {
      if (d->need_to_upload()) { d->upload(); }
      d->draw();
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

  glfwTerminate();
}

}  // namespace saa
