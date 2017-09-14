#include "saa/drawing/circles.hpp"

#include <iostream>  // TODO rm

#include "config.hpp"

namespace saa {

namespace {

constexpr char VERT_SHADER_PATH[] = SAA_SHADER_PATH "/circles.vs.glsl";
constexpr char FRAG_SHADER_PATH[] = SAA_SHADER_PATH "/circles.fs.glsl";

}  // namespace

Circle::Circle(const Vec3f &position, const float radius, const bool filled, const Vec3f &normal)
    : position{position}, normal{normal}, radius{radius}, filled{filled} {}

Circles::Circles(Window &window)
    : shader_{window.create_shader(VERT_SHADER_PATH, FRAG_SHADER_PATH)},
      circles_verts_{},
      circles_indices_{},
      circles_changed_{false},
      vao_{0},
      vbo_{0},
      ebo_{0} {}

void Circles::add_circle(const Circle &circle) {
    const auto old_cols = circles_verts_.cols();
    circles_verts_.conservativeResize(circles_verts_.rows(), old_cols + 4);

    const uint top_left_idx = circles_verts_.cols() - 4;
    const Vec3f top_left{
        circle.position[0] - circle.radius, circle.position[1] + circle.radius, circle.position[2]};
    circles_verts_.col(top_left_idx) << top_left, circle.radius, circle.position[0],
        circle.position[1];

    const uint top_right_idx = circles_verts_.cols() - 3;
    const Vec3f top_right{
        circle.position[0] + circle.radius, circle.position[1] + circle.radius, circle.position[2]};
    circles_verts_.col(top_right_idx) << top_right, circle.radius, circle.position[0],
        circle.position[1];

    const uint bottom_left_idx = circles_verts_.cols() - 2;
    const Vec3f bottom_left{
        circle.position[0] - circle.radius, circle.position[1] - circle.radius, circle.position[2]};
    circles_verts_.col(bottom_left_idx) << bottom_left, circle.radius, circle.position[0],
        circle.position[1];

    const uint bottom_right_idx = circles_verts_.cols() - 1;
    const Vec3f bottom_right{
        circle.position[0] + circle.radius, circle.position[1] - circle.radius, circle.position[2]};
    circles_verts_.col(bottom_right_idx) << bottom_right, circle.radius, circle.position[0],
        circle.position[1];

    circles_indices_.conservativeResize(circles_indices_.rows(), circles_indices_.cols() + 2);
    circles_indices_.col(circles_indices_.cols() - 2) << top_left_idx, top_right_idx,
        bottom_left_idx;
    circles_indices_.col(circles_indices_.cols() - 1) << top_right_idx, bottom_left_idx,
        bottom_right_idx;

    std::cout << circles_indices_ << "\n" << circles_verts_ << std::endl;

    circles_changed_ = true;
}

void Circles::initialize() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
}

void Circles::draw(const Mat4f &clip_from_world) {
    if (circles_verts_.size() == 0) {
        return;
    }
    shader_.use();
    shader_.set_clip_from_local(clip_from_world);
    glBindVertexArray(vao_);
    {  //
        glDrawElements(GL_TRIANGLES, circles_indices_.size(), GL_UNSIGNED_INT, 0);
    }  //
    glBindVertexArray(0);
}

void Circles::upload() {
    if (circles_verts_.size() == 0) {
        return;
    }

    glBindVertexArray(vao_);
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER,
                     circles_verts_.size() * sizeof(float),
                     circles_verts_.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     circles_indices_.size() * sizeof(float),
                     circles_indices_.data(),
                     GL_STATIC_DRAW);

        // vertices
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        // radius
        glVertexAttribPointer(
            1, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        // center
        glVertexAttribPointer(
            2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(4 * sizeof(float)));
        glEnableVertexAttribArray(0);
    }
    glBindVertexArray(0);

    circles_changed_ = false;
}

bool Circles::need_to_upload() { return circles_changed_; }

}  // namespace saa
