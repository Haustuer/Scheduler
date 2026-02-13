
# PeriodicTask

Lightweight, header-only periodic task helper built on `millis()` for Arduino-compatible boards.

[![Arduino CI](https://github.com/Haustuer/Scheduler/actions/workflows/arduino.yml/badge.svg)](https://github.com/Haustuer/Scheduler/actions/workflows/arduino.yml)
[![PlatformIO CI](https://github.com/Haustuer/Scheduler/actions/workflows/platformio.yml/badge.svg)](https://github.com/Haustuer/Scheduler/actions/workflows/platformio.yml)

## Features
- Wrap-around safe scheduling using signed subtraction
- Optional catch-up vs. coalesced overdue runs
- Context pointer passed to callback (`void (*)(void*)`)
- Header-only (`src/PeriodicTask.h`)

## Install
### Arduino IDE
1. (Recommended) **Library Manager**: once the repo is published and tagged, search for **PeriodicTask**.
2. Or add ZIP: **Sketch → Include Library → Add .ZIP Library...** and select a release from GitHub.

### PlatformIO
Add to `platformio.ini`:
```ini
lib_deps =
  Haustuer/Scheduler/PeriodicTask@^1.0.0
```
Or copy the folder into your project `lib/`.

## Quick start
```cpp
#include <PeriodicTask.h>

void blink(void *) {
  int pin = 13;
  digitalWrite(pin, !digitalRead(pin));
}

PeriodicTask blinker(500, blink);

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  uint32_t now = millis();
  blinker.poll(now);
}
```

## API
- `bool poll(uint32_t now)` – run if due (returns whether it ran)
- `bool tick()` – convenience wrapper calling `poll(millis())`
- `void start(bool immediate = true)` / `void stop()` / `void enable(bool on = true)`
- `void setInterval(uint32_t intervalMs, bool reschedule = true)`
- `void setPhase(uint32_t delayMs)` (delay next run from *now*)
- `void setContext(void* ctx)` / `void runOnce()`
- `bool due() const`
- `setCoalesceOverdue(bool on)` / `getCoalesceOverdue() const`

## Notes
- **Overflow-safe**: uses `(int32_t)(now - _nextDue) >= 0` to handle `millis()` wrap-around.
- **Coalesce vs strict**: When coalescing, late polls run once and schedule `now + interval`. When strict, it advances in exact multiples—may run several times in one `poll` if late.

## License
MIT © Hauke Stuerenburg
