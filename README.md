# aeinfo-modern

Modernized version of **aeinfo 2.4** for the **Aqua Computer Aquaero 4**.

## Status

- Debian 13 support
- GCC 14 compatible
- USB communication working
- Exit crash fixed
- aquaerod working

## Goals

- Clean up the original source
- Document the USB protocol
- ESP32 USB Host support
- Modern Web UI
- REST API
- MQTT (optional)
- Native FHEM support

## Build

```bash
make
```

## Repository

```
docs/      Documentation
example/   Example code
htdocs/    Original web templates
legacy/    Original Windows and historical files
```

## Credits

Original project: Christian Unger

Modernization:
https://github.com/skydanc3r/aeinfo-modern