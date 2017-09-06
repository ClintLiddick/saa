#include <cstdlib>
#include <iostream>

#include "saa/drawing/points.hpp"
#include "saa/types.hpp"
#include "saa/window.hpp"

int main() {
  std::cout << "hello" << std::endl;

  saa::Window win{900, 600, "Hello"};
  win.open();
  std::cout << "opened" << std::endl;

  saa::Mat3Xf points(3, 3);
  points.col(0) << 10, 10, 0;
  points.col(1) << 0, 7, 0;
  points.col(2) << 5, 5, 0;

  auto p = std::make_unique<saa::Points>(win);
  p->set_points(points);
  win.set_background_color({0.2,0.2,0.2,1.0});
  win.add_drawable(std::move(p));

  std::cout << "start spin" << std::endl;
  win.spin();
  win.close();
  std::cout << "goodbye" << std::endl;
  return EXIT_SUCCESS;
}
