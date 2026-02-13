#include <PeriodicTask.h>

// (1) Print "1 hallo welt"
static void cbHalloWelt(void *) {
  Serial.println(F("Hallo welt"));
}

// (2) Print "2 <millis> <counter>"
static void cbStatus(void *) {
  static uint32_t calls = 1;
  ++calls;
  Serial.print(F("Called for the"));
  Serial.print(calls);
  Serial.print(F(" time @ "));
  Serial.println(millis());
  
}

static PeriodicTask tHello(1000, cbHalloWelt); // every 1000 ms
static PeriodicTask tStatus(500,  cbStatus);   // every 500 ms

void setup() {
  Serial.begin(115200);
  // while (!Serial) {} // (optional) for native USB boards
  // Optional: avoid burst catch-up if loop stalls
  // tHello.setCoalesceOverdue(true);
  // tStatus.setCoalesceOverdue(true);
}

void loop() {
  uint32_t now = millis();
  tHello.poll(now);
  tStatus.poll(now);
}
