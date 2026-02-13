
# Contributing
Thanks for considering a contribution!

## Workflow
1. Fork the repo and create a feature branch.
2. Run CI locally if possible (Arduino CLI / PlatformIO) and ensure examples compile.
3. Open a Pull Request describing your change and test coverage.

## Code style
- Keep the library header-only and Arduino-friendly.
- Prefer small, self-contained examples for docs.

## Tests
- At minimum, the `examples/` must compile for AVR and ESP32 targets.
- If adding features, consider a minimal example to demonstrate behavior.
