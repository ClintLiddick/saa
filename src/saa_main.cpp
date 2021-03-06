#include <cstdlib>
#include <iostream>

// #include "saa/drawing/points.hpp"
#include "saa/drawing/circles.hpp"
#include "saa/types.hpp"
#include "saa/window.hpp"

int main() {
    saa::Window win{900, 600, "Hello"};
    win.open();

    // saa::Mat3Xf points(3, 5);
    // points.col(0) << 10, 10, 0;
    // points.col(1) << 0, 7, 0;
    // points.col(2) << 5, 5, 0;
    // points.col(3) << 3, 3, 0;
    // points.col(4) << 2, 4, 0;

    // auto p = std::make_unique<saa::Points>(win, 2);
    // p->set_points(points);

    auto c = std::make_unique<saa::Circles>(win);
    c->add_circle({{0, 0, 0}, 2});

    win.set_background_color({0.2, 0.2, 0.2, 1.0});
    win.add_drawable(std::move(c));

    win.spin();
    return EXIT_SUCCESS;
}
