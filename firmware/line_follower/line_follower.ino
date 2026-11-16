#include "controller.h"

// Hardware boundary: Uno R3, QTR-1A outputs, Pololu DRV8833 carrier #2130.
const uint8_t sensorPins[] = {A0, A1, A2}; // left to right, facing forward
const uint8_t leftPwm = 5, leftLow = 4, rightPwm = 6, rightLow = 7;
const uint8_t sleepPin = 8, faultPin = 2;
robot::Calibration calibration = {};
bool armed = false;
bool whiteCaptured = false, blackCaptured = false;

robot::Readings readSensors() {
  robot::Readings readings = {};
  for (uint8_t i = 0; i < 3; ++i) readings.value[i] = analogRead(sensorPins[i]);
  return readings;
}

void stopMotors() {
  digitalWrite(sleepPin, LOW);
  analogWrite(leftPwm, 0);
  analogWrite(rightPwm, 0);
  armed = false;
}

void capture(uint16_t* destination) {
  stopMotors();
  uint32_t sums[3] = {};
  for (uint8_t sample = 0; sample < 32; ++sample) {
    const auto readings = readSensors();
    for (uint8_t i = 0; i < 3; ++i) sums[i] += readings.value[i];
    delay(2);
  }
  for (uint8_t i = 0; i < 3; ++i) destination[i] = sums[i] / 32;
  Serial.println(F("Captured. Calibration must have black >= white + 100 on all sensors."));
}

void processSerial() {
  if (!Serial.available()) return;
  switch (Serial.read()) {
    case 'w': capture(calibration.white); whiteCaptured = true; break;
    case 'b': capture(calibration.black); blackCaptured = true; break;
    case 'g':
      armed = whiteCaptured && blackCaptured && robot::valid(calibration) && digitalRead(faultPin) == HIGH;
      Serial.println(armed ? F("Armed; requires visible line.") : F("Refused: calibration/fault."));
      break;
    case 'x': stopMotors(); break;
    case '\r': case '\n': break;
    default: stopMotors(); Serial.println(F("Unknown command: stopped."));
  }
}

void setup() {
  digitalWrite(sleepPin, LOW);
  pinMode(sleepPin, OUTPUT);
  const uint8_t outputs[] = {leftPwm, leftLow, rightPwm, rightLow};
  for (uint8_t pin : outputs) { digitalWrite(pin, LOW); pinMode(pin, OUTPUT); }
  pinMode(faultPin, INPUT_PULLUP);
  stopMotors();
  Serial.begin(115200);
  Serial.println(F("STOPPED. w=white b=black g=go x=stop. Calibration lost on reset."));
}

void loop() {
  processSerial();
  const auto readings = readSensors();
  const auto command = robot::follow(readings, calibration);
  if (!armed || digitalRead(faultPin) == LOW || command.left == 0) {
    stopMotors();
  } else {
    analogWrite(leftPwm, command.left);
    analogWrite(rightPwm, command.right);
    digitalWrite(sleepPin, HIGH);
  }
  // About 50 Hz; serial telemetry allows calibration checks while stopped.
  static uint8_t frame = 0;
  if (++frame == 10) {
    frame = 0;
    for (uint8_t i = 0; i < 3; ++i) { Serial.print(readings.value[i]); Serial.print(' '); }
    Serial.println(armed ? F("RUN") : F("STOP"));
  }
  delay(20);
}
