#include <cstdlib>
#include <iostream>

#include "saa/drawing/points.hpp"
#include "saa/types.hpp"
#include "saa/window.hpp"

int main() {
  std::cout << "hello" << std::endl;

  saa::Window win{900, 600, "Hello"};
  win.open();

  saa::Mat3Xf points(3, 1);
  points << 0, 0, 0;

  auto p = std::make_unique<saa::Points>(win);
  // win.add_drawable(std::move(p));

  win.spin();
  win.close();
  std::cout << "goodbye" << std::endl;
  return EXIT_SUCCESS;
}
