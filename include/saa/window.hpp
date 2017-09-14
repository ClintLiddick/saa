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
    Shader create_shader(const std::string &vert_shader_path, const std::string &frag_shader_path);

 private:
    void initialize();
    void spin_gl();
    void update_camera();

    void key_callback(const int key, const int scancode, const int action, const int mods);

    void scroll_callback(const double xoffset, const double yoffset);

    void mouse_button_callback(const int button, const int action, const int mods);

    void cursor_position_callback(const double xpos, const double ypos);

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
    bool ortho_;
    std::vector<std::unique_ptr<Drawable>> drawables_;
    float fov_;
    Vec3f camera_pos_;
    Vec3f camera_target_pos_;

    bool right_mouse_pressed_;

    float prev_cursor_x_;
    float prev_cursor_y_;
    float cursor_x_;
    float cursor_y_;
};

}  // namespace saa
