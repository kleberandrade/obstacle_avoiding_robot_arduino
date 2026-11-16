#include "controller.h"
#include <cassert>
#include <iostream>
using namespace robot;
int main() {
  Calibration cal = {{100, 120, 140}, {900, 920, 940}};
  assert(valid(cal));
  for (int mask = 0; mask < 8; ++mask) {
    Readings r = {{static_cast<uint16_t>((mask & 4) ? 900 : 100), static_cast<uint16_t>((mask & 2) ? 920 : 120),
                   static_cast<uint16_t>((mask & 1) ? 940 : 140)}};
    const Command c = follow(r, cal);
    if (mask == 0 || mask == 5 || mask == 7) assert(c.left == 0 && c.right == 0);
    if (mask == 2) assert(c.left == 120 && c.right == 120);
    if (mask == 4 || mask == 6) assert(c.left == 45 && c.right == 120);
    if (mask == 1 || mask == 3) assert(c.left == 120 && c.right == 45);
  }
  assert(follow({{100, 520, 140}}, cal).left == 120); // inclusive midpoint
  assert(follow({{100, 519, 140}}, cal).left == 0);
  Calibration bad = cal;
  bad.black[0] = 199;
  assert(!valid(bad));
  assert(follow({{100, 920, 140}}, bad).right == 0);
  bad.black[0] = 200;
  assert(valid(bad)); // minimum contrast, exactly 100 ADC counts
  bad.white[1] = 1024;
  assert(!valid(bad));
  assert(follow({{1024, 920, 140}}, cal).right == 0);
  bad = cal;
  bad.black[2] = 1024;
  assert(!valid(bad));
  bad = cal;
  bad.black[1] = 0;
  assert(!valid(bad));
  std::cout << "Controller behavior passed\n";
}
