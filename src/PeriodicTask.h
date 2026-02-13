\
#pragma once
#include <Arduino.h>

class PeriodicTask
{
public:
  using Callback = void (*)(void *);

  /**
   * Construct a periodic task.
   * @param intervalMs  Period in milliseconds (must be > 0; clamped to 1 if 0).
   * @param cb          Callback signature: void (*)(void*)
   * @param ctx         Opaque pointer passed to the callback (default nullptr)
   * @param startNow    If true, first run may occur immediately on next poll.
   *                    If false, first run is scheduled for now + interval.
   */
  PeriodicTask(uint32_t intervalMs, Callback cb, void *ctx = nullptr, bool startNow = true)
      : _interval(intervalMs ? intervalMs : 1),
        _cb(cb),
        _ctx(ctx),
        _enabled(true)
  {
    const uint32_t now = _now();
    _nextDue = startNow ? now : (now + _interval);
  }

  /**
   * Poll with a provided timestamp (shared `now` for multiple tasks).
   * Call this every loop with the same `now` for all tasks:
   *   uint32_t now = millis();
   *   task.poll(now);
   * @return true if the callback ran during this call.
   */
  bool poll(uint32_t now)
  {
    if (!_enabled || !_cb)
      return false;
    if (_due(now))
    {
      _invoke();

      if (_coalesceOverdue)
      {
        _nextDue = now + _interval;
      }
      else
      {
        // Strict periodic schedule: advance by exact multiples to catch up
        do
        {
          _nextDue += _interval;
        } while (_due(now));
      }
      return true;
    }
    return false;
  }

  /**
   * Convenience: uses millis() internally (if available).
   */
  bool tick()
  {
    return poll(_now());
  }

  /**
   * Start (enable) the task; optionally run immediately (default) or phase it.
   */
  void start(bool immediate = true)
  {
    _enabled = true;
    const uint32_t now = _now();
    _nextDue = immediate ? now : (now + _interval);
  }

  /**
   * Stop (disable) the task; keeps schedule state but won't run while disabled.
   */
  void stop() { _enabled = false; }

  /**
   * Enable/disable the task.
   * When re‑enabling, next due is set to now (prevents long catch‑up).
   */
  void enable(bool on = true)
  {
    _enabled = on;
    if (on)
      _nextDue = _now();
  }

  /**
   * Change the interval; optionally reschedule from now (default).
   */
  void setInterval(uint32_t intervalMs, bool reschedule = true)
  {
    if (intervalMs == 0)
      intervalMs = 1;
    _interval = intervalMs;
    if (reschedule)
      _nextDue = _now() + _interval;
  }

  /**
   * Delay first/next run by a specific phase (ms) from now.
   */
  void setPhase(uint32_t delayMs)
  {
    _nextDue = _now() + delayMs;
  }

  /**
   * Set or change the context pointer passed to the callback.
   */
  void setContext(void *ctx) { _ctx = ctx; }

  /**
   * Run the callback once right now (does not alter schedule).
   */
  void runOnce()
  {
    if (_cb)
      _cb(_ctx);
  }

  /**
   * Returns true if enabled and due as of "now".
   */
  bool due() const
  {
    if (!_enabled)
      return false;
    return _due(_now());
  }

  uint32_t getInterval() const { return _interval; }
  uint32_t getNextDue() const { return _nextDue; }
  bool getEnabled() const { return _enabled; }

  void setCoalesceOverdue(bool on) { _coalesceOverdue = on; }
  bool getCoalesceOverdue() const { return _coalesceOverdue; }

private:
  uint32_t _interval; // ms
  Callback _cb;
  void *_ctx;
  uint32_t _nextDue; // absolute time in ms
  bool _enabled;
  bool _coalesceOverdue = false;

  static uint32_t _now()
  {
    return millis();
  }

  // Handles wrap‑around by using signed subtraction
  bool _due(uint32_t now) const
  {
    return (int32_t)(now - _nextDue) >= 0;
  }

  void _invoke()
  {
    Callback cb = _cb; // local copy guards against changes by callback
    void *ctx = _ctx;
    if (cb)
      cb(ctx);
  }
};
