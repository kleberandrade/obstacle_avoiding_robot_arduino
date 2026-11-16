#include <cassert>
#include <cstdint>
#include <deque>
#include <iostream>
#define F(text) text
const uint8_t A0 = 14, A1 = 15, A2 = 16;
const int LOW = 0, HIGH = 1, OUTPUT = 1, INPUT_PULLUP = 2;
int pins[20] = {}, adc[3] = {};
int analogRead(uint8_t pin) { return adc[pin - A0]; }
int digitalRead(uint8_t pin) { return pins[pin]; }
void digitalWrite(uint8_t pin, int value) { pins[pin] = value; }
void analogWrite(uint8_t pin, int value) { pins[pin] = value; }
void pinMode(uint8_t pin, int mode) { if (mode == INPUT_PULLUP) pins[pin] = HIGH; }
void delay(int) {}
struct SerialFake {
  std::deque<char> input;
  void begin(int) {}
  bool available() { return !input.empty(); }
  char read() { char c = input.front(); input.pop_front(); return c; }
  template <typename T> void print(T) {}
  template <typename T> void println(T) {}
} Serial;
#include "../firmware/line_follower/line_follower.ino"
void send(char c) { Serial.input.push_back(c); loop(); }
void surface(int left, int center, int right) { adc[0]=left; adc[1]=center; adc[2]=right; }
void stopped() { assert(!armed && pins[8] == LOW && pins[5] == 0 && pins[6] == 0); }
int main() {
  setup(); stopped();
  send('g'); stopped(); // cannot run before calibration
  surface(900, 900, 900); send('b');
  surface(100, 900, 100); send('g'); stopped(); // both captures required
  surface(100, 100, 100); send('w'); stopped();
  surface(900, 900, 900); send('b'); stopped();
  surface(100, 900, 100); send('g');
  assert(armed && pins[8] == HIGH && pins[5] == 120 && pins[6] == 120);
  surface(900, 100, 100); loop(); assert(pins[5] == 45 && pins[6] == 120);
  surface(100, 100, 100); loop(); stopped();
  surface(100, 900, 100); loop(); stopped(); // loss latches stop
  send('g'); assert(armed);
  send('x'); stopped();
  send('g'); pins[2] = LOW; loop(); stopped();
  send('g'); stopped(); // active fault refuses restart
  pins[2] = HIGH; send('g'); assert(armed);
  send('?'); stopped();
  send('g'); send('w'); stopped(); // calibration stops motors
  surface(900, 900, 900); send('g'); stopped(); // ambiguous line
  std::cout << "Firmware boundary behavior passed (simulated I/O)\n";
}
