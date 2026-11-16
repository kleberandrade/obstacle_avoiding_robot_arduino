# Validation record

## Executed locally

- `make test example` with Apple Clang: passed, using C++11 and
  `-Wall -Wextra -Werror -pedantic`.
- Controller tests: all eight sensor patterns, inclusive midpoint, one-count-below
  midpoint, 99/100-count minimum contrast, reversed calibration, ADC out of range.
- Sketch tests with simulated Arduino I/O: stopped startup, rejection before both
  calibration captures, forward/left commands, lost-line stop and latched restart,
  explicit stop, active driver fault, unknown serial command, capture while running
  and ambiguous line stop.
- Arduino CLI **1.6.0**, AVR core **1.8.8**, `arduino:avr:uno` cross-compilation:
  passed; **3776 bytes flash, 210 bytes global SRAM** reported by the compiler.
  These are static build sizes, not a runtime stack measurement.
- Original SVG rendered with macOS Quick Look and visually inspected. Local
  document links and SVG XML checked; `git diff --check` passed.

The workflow in `.github/workflows/ci.yml` repeats the host tests/example and Uno
compile on Ubuntu 24.04. The README CI badge reports the remote workflow's actual
status; it is not a static claim about test success. Toolchain releases are pinned
in the workflow/profile. The hardware compatibility references are recorded in
[HARDWARE.md](HARDWARE.md).

## Limits of this evidence

The host sketch test is a fake I/O harness; it does not emulate AVR timing,
electrical behavior, motor dynamics, ADC noise or resets. No board was connected,
no firmware upload was performed, and no physical circuit, battery runtime,
tracking speed, stopping distance, thermal behavior or driver fault was measured.
The [bench acceptance procedure](HARDWARE.md#bench-acceptance-procedure-must-be-performed-by-the-builder)
is required before operating an assembled robot.

`crapper`, `dryer` and `mutator` are installed, but their documented supported
languages exclude C++ and Arduino sketches. They were not used to manufacture
metrics for unsupported code. No mutation score or coverage percentage is claimed.
The tests were written against behavior and include an observed regression failure
for incomplete calibration before the fix, as recorded in [the work log](WORK_LOG.md).
