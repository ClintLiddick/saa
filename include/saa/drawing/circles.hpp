#pragma once

#include <vector>

#include "saa/drawable.hpp"
#include "saa/gl.h"
#include "saa/types.hpp"
#include "saa/window.hpp"

namespace saa {

struct Circle {
    Circle(const Vec3f &position,
           const float radius,
           const bool filled = true,
           const Vec3f &normal = {0, 0, 1});

    Vec3f position;
    Vec3f normal;
    float radius;
    bool filled;
};

class Circles final : public Drawable {
 public:
    explicit Circles(Window &window);
    virtual ~Circles() = default;

    Circles(const Circles &) = delete;
    Circles &operator=(const Circles &) = delete;

    Circles(Circles &&) = default;
    Circles &operator=(Circles &&) = default;

    // void set_circles(const Mat3Xf &circles);
    // void add_circle(const std::vector<Circle> &circles);
    void add_circle(const Circle &circle);

    void initialize() override;
    void draw(const Mat4f &clip_from_world) override;
    void upload() override;
    bool need_to_upload() override;

 private:
    Shader shader_;
    Mat6Xf circles_verts_;
    Mat3Xu circles_indices_;
    bool circles_changed_;
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
};

}  // namespace saa
