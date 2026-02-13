
#include <PeriodicTask.h>

void togglePin(void* ctx) {
  int pin = (int)(intptr_t)ctx;
  digitalWrite(pin, !digitalRead(pin));
}

PeriodicTask led(500, togglePin, (void*)(intptr_t)LED_BUILTIN, true);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  uint32_t now = millis();
  led.poll(now);
}
