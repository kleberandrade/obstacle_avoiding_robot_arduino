#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H
#include <stdint.h>
namespace robot {
struct Readings { uint16_t value[3]; };
struct Calibration { uint16_t white[3]; uint16_t black[3]; };
struct Command { uint8_t left; uint8_t right; };
inline bool valid(const Calibration& cal) {
  for (uint8_t i = 0; i < 3; ++i) {
    if (cal.white[i] > 1023 || cal.black[i] > 1023) return false;
    if (cal.black[i] < cal.white[i] + 100U) return false;
  }
  return true;
}
// High voltage means dark on QTR-1A. Stop on loss or ambiguous patterns.
inline Command follow(const Readings& readings, const Calibration& cal) {
  if (!valid(cal)) return {0, 0};
  uint8_t mask = 0;
  for (uint8_t i = 0; i < 3; ++i) {
    if (readings.value[i] > 1023) return {0, 0};
    mask = static_cast<uint8_t>(mask << 1);
    if (2U * readings.value[i] >= cal.white[i] + cal.black[i]) mask |= 1;
  }
  switch (mask) {
    case 2: return {120, 120};
    case 4: case 6: return {45, 120};
    case 1: case 3: return {120, 45};
    default: return {0, 0};
  }
}
}
#endif
