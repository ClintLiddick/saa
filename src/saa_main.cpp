#include <cstdlib>
#include <iostream>

#include "saa/window.hpp"

int main() {
  std::cout << "hello" << std::endl;

  saa::Window win{900, 600, "Hello"};
  win.open();
  win.spin();
  win.close();

  std::cout << "goodbye" << std::endl;
  return EXIT_SUCCESS;
}
