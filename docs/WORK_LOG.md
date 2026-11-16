# Work log

- Inspected the cloned repository: README, MIT license, no firmware or local changes; no applicable AGENTS.md. Kept the remote name and original copyright.
- Applied ub-mode: separated sensor-to-wheel decisions from hardware I/O; wrote controller tests first, observed an assertion failure against a stop-only implementation, then implemented the truth table and calibration boundaries.
- Chose Uno R3, three QTR-1A analog sensors and Pololu DRV8833 #2130 after checking manufacturer documentation. Independent USB logic power and switched 4-cell NiMH motor power avoid motor current through the Uno.
- Added a host example and simulated Arduino boundary tests for calibration, explicit start, stop, fault and lost-line restart. A new test exposed black-only calibration being accepted with zero-initialized white values; added explicit capture state and reran tests.
- Checked installed crapper, dryer and mutator help: none supports C++/Arduino. No CRAP, duplication or mutation score is claimed. Used compiler warnings and behavioral tests instead.
- Selected official Arduino CLI 1.6.0 and AVR core 1.8.8; pinned the build profile and CI. The bundled AVR compiler is managed by the core, rather than replaced independently.
- Added English README, hardware/BOM/connection/calibration/bench instructions, limitations and original SVG; rendered and inspected the SVG, corrected preview framing, and checked local links.
- Added Ubuntu host/AVR CI and truthful README badges for C++11, Uno R3, CLI 1.6.0, AVR 1.8.8, MIT and the real master-branch workflow. Updated checkout to the current official v7.0.1 release.
- User subsequently authorized publishing to the principal branch after tests and updating GitHub About. Confirmed origin and local master; publication follows completed local checks, without force push.
