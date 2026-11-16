#include "controller.h"
#include <iostream>
int main() {
  const robot::Calibration cal = {{100, 100, 100}, {900, 900, 900}};
  const robot::Readings track[] = {{{100, 900, 100}}, {{900, 100, 100}},
                                  {{100, 100, 900}}, {{100, 100, 100}}};
  for (const auto& frame : track) {
    const auto command = robot::follow(frame, cal);
    std::cout << "left=" << int(command.left) << " right=" << int(command.right) << '\n';
  }
}
