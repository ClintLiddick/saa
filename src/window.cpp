#include "saa/window.hpp"

#include <chrono>
#include <stdexcept>

#include "saa/projection.hpp"

namespace saa {

//
// Public
//

Window::Window(const int width, const int height, const std::string &title)
    : width_{width}
    , height_{height}
    , title_{title}
    , initialized_{false}
    , should_close_{false}
    , bg_color_{0.0, 0.0, 0.0, 1.0} {}

Window::~Window() { should_close_ = true; }

void Window::open() {
  thread_ = std::thread{[this]() {
    initialize();
    spin_gl();
  }};

  while (true) {
    {
      const std::lock_guard<std::mutex> lck{mutex_};
      if (initialized_) { break; }
    }
    std::this_thread::sleep_until(std::chrono::steady_clock::now()
                                  + MS_PER_FRAME);
  }
}

void Window::close() {
  const std::lock_guard<std::mutex> lck{mutex_};
  should_close_ = true;
}

void Window::spin() { thread_.join(); }

//
// Drawing utility
//

void Window::set_background_color(const Vec4f &color) {
  const std::lock_guard<std::mutex> lck{mutex_};
  bg_color_ = color;
}

void Window::add_drawable(std::unique_ptr<Drawable> drawable) {
  const std::lock_guard<std::mutex> lck{mutex_};
  glfwMakeContextCurrent(window_);
  drawables_.push_back(std::move(drawable));
  drawables_.back()->initialize();
  glfwMakeContextCurrent(nullptr);
}

Shader Window::create_shader(const std::string &vert_shader_path,
                             const std::string &frag_shader_path) {
  const std::lock_guard<std::mutex> lck{mutex_};
  glfwMakeContextCurrent(window_);
  Shader shader{vert_shader_path, frag_shader_path};
  glfwMakeContextCurrent(nullptr);
  return shader;
}

//
// Private
//

void Window::initialize() {
  const std::lock_guard<std::mutex> lck{mutex_};
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

  initialized_ = true;
}

void Window::spin_gl() {
  // // clang-format off
  // clip_from_world <<

  // // clang-format on
  while (true) {
    const auto start = std::chrono::steady_clock::now();
    {
      const std::lock_guard<std::mutex> lck{mutex_};
      glfwMakeContextCurrent(window_);

      if (should_close_ || glfwWindowShouldClose(window_)) { break; }

      int width, height;
      glfwGetFramebufferSize(window_, &width, &height);
      const float aspect = (float)width / height;
      glViewport(0, 0, width, height);
      // Fix vertical aspect ratio, and let horizontal size determine view
      // width, not ratio.
      const Mat4f clip_from_world =
          ortho_projection(-100 * aspect, 100 * aspect, -100, 100, -100, 100);

      glClearColor(bg_color_[0], bg_color_[1], bg_color_[2], bg_color_[3]);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      for (auto &d : drawables_) {
        if (d->need_to_upload()) { d->upload(); }
        d->draw(clip_from_world);
      }

      glfwSwapBuffers(window_);
      glfwPollEvents();
      glfwMakeContextCurrent(nullptr);
    }

    std::this_thread::sleep_until(start + MS_PER_FRAME);
  }

  glfwTerminate();
}

constexpr std::chrono::milliseconds Window::MS_PER_FRAME;
}  // namespace saa
