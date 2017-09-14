#include "saa/window.hpp"

#include <chrono>
#include <stdexcept>

#include <iostream> // TODO rm

#include "saa/camera.hpp"
#include "saa/projection.hpp"

namespace saa {

//
// Public
//

Window::Window(const int width, const int height, const std::string &title)
    : width_{width},
      height_{height},
      title_{title},
      initialized_{false},
      should_close_{false},
      bg_color_{0.0, 0.0, 0.0, 1.0},
      ortho_{false},
      fov_{45.0},
      camera_pos_{0, 0, 3},
      camera_target_pos_{0, 0, 0},
      right_mouse_pressed_{false},
      prev_cursor_x_{0},
      prev_cursor_y_{0},
      cursor_x_{0},
      cursor_y_{0} {}

Window::~Window() { should_close_ = true; }

void Window::open() {
    thread_ = std::thread{[this]() {
        initialize();
        spin_gl();
    }};

    while (true) {
        {
            const std::lock_guard<std::mutex> lck{mutex_};
            if (initialized_) {
                break;
            }
        }
        std::this_thread::sleep_until(std::chrono::steady_clock::now() + MS_PER_FRAME);
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

    if (!glfwInit()) {
        throw std::runtime_error{"Unable to initialize GLFW"};
    }

    window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
    if (!window_) {
        throw std::runtime_error{"Unable to create window."};
    }

    glfwMakeContextCurrent(window_);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error{"Unable to load OpenGL context."};
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Make class instance available in C-style function poitner callbacks.
    glfwSetWindowUserPointer(window_, static_cast<void *>(this));

    // Register callbacks

    glfwSetKeyCallback(window_,
                       [](GLFWwindow *window,
                          const int key,
                          const int scancode,
                          const int action,
                          const int mods) {
                           Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
                           self->key_callback(key, scancode, action, mods);
                       });

    glfwSetScrollCallback(
        window_, [](GLFWwindow *window, const double xoffset, const double yoffset) {
            Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
            self->scroll_callback(xoffset, yoffset);
        });

    glfwSetMouseButtonCallback(
        window_, [](GLFWwindow *window, const int button, const int action, const int mods) {
            Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
            self->mouse_button_callback(button, action, mods);
        });

    glfwSetCursorPosCallback(window_, [](GLFWwindow *window, const double xpos, const double ypos) {
        Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
        self->cursor_position_callback(xpos, ypos);
    });

    initialized_ = true;
}

void Window::update_camera() {
    if (right_mouse_pressed_) {
        // TODO(clint): probably not correct logic
        const float dx = cursor_x_ - prev_cursor_x_;
        camera_target_pos_[0] -= dx * 0.01;
    }
}

void Window::spin_gl() {
    while (true) {
        const auto start = std::chrono::steady_clock::now();
        {
            const std::lock_guard<std::mutex> lck{mutex_};
            glfwMakeContextCurrent(window_);

            if (should_close_ || glfwWindowShouldClose(window_)) {
                break;
            }

            int width, height;
            glfwGetFramebufferSize(window_, &width, &height);
            const float aspect = (float)width / height;
            glViewport(0, 0, width, height);

            update_camera();

            // Fix vertical aspect ratio, and let horizontal size determine view
            // width, not ratio.
            const Mat4f clip_from_view =
                ortho_ ? ortho_projection(-100 * aspect, 100 * aspect, -100, 100, 0.1, 100)
                       : perspective_projection(fov_, aspect, 10, 100);
            const Mat4f view_from_world = look_at(camera_pos_, camera_target_pos_, {0, 1, 0});
            const Mat4f clip_from_world = clip_from_view * view_from_world;

            glClearColor(bg_color_[0], bg_color_[1], bg_color_[2], bg_color_[3]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            for (auto &d : drawables_) {
                if (d->need_to_upload()) {
                    d->upload();
                }
                d->draw(clip_from_world);
            }

            glfwSwapBuffers(window_);
            glfwPollEvents();
            glfwMakeContextCurrent(nullptr);
        }

        std::this_thread::sleep_until(start + MS_PER_FRAME);
    }

    const std::lock_guard<std::mutex> lck{mutex_};
    glfwMakeContextCurrent(window_);
    glfwTerminate();
}

void Window::key_callback(const int key,
                          const int /* scancode */,
                          const int action,
                          const int /* mods */) {
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
        should_close_ = true;
    }
    if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
        ortho_ = !ortho_;
    }
}

void Window::scroll_callback(const double /* xoffset */, const double yoffset) {
    if (fov_ >= 1.0 && fov_ <= 45.0) {
        fov_ -= yoffset;
    }
    if (fov_ <= 1.0) {
        fov_ = 1.0;
    }
    if (fov_ >= 45.0) {
        fov_ = 45.0;
    }
}

void Window::mouse_button_callback(const int button, const int action, const int /* mods */) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            right_mouse_pressed_ = true;
        } else {
            right_mouse_pressed_ = false;
        }
    }
}

void Window::cursor_position_callback(const double xpos, const double ypos) {
    prev_cursor_x_ = cursor_x_;
    prev_cursor_y_ = cursor_y_;
    cursor_x_ = static_cast<float>(xpos);
    cursor_y_ = static_cast<float>(ypos);
}

constexpr std::chrono::milliseconds Window::MS_PER_FRAME;
}  // namespace saa
