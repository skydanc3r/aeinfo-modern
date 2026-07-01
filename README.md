# aeinfo-modern

Modernized version of **aeinfo 2.4** for the **Aqua Computer Aquaero 4**.

This project continues the original **aeinfo** software by **Christian Unger** and aims to preserve and modernize it for current Linux systems while providing a foundation for future development.

---

## Current Status

- ✅ Debian 13 compatible
- ✅ GCC 14 compatible
- ✅ USB communication working
- ✅ Exit segmentation fault fixed
- ✅ aquaerod builds successfully
- ✅ GitHub project established

---

## Project Goals

### Phase 1
- Modernize and stabilize the original code
- Remove obsolete components
- Improve documentation

### Phase 2
- Document the USB protocol
- Refactor the codebase
- Modularize the project

### Phase 3
- ESP32 USB Host implementation
- Modern responsive Web Interface
- REST API

### Phase 4
- MQTT support
- Home Assistant integration
- Native FHEM module

---

## Build

```bash
make
```

---

## Repository Structure

```text
docs/       Documentation
example/    Example applications
htdocs/     Original web interface templates
legacy/     Historical Windows files and old project resources
```

---

## License

This project is licensed under the **GNU Lesser General Public License (LGPL)**.

The original source code is:

Copyright (C) 2006–2007 Christian Unger

This repository contains a modernized continuation of the original project.

The original license and copyright notices are preserved.

---

## Credits

Original project

Christian Unger

Modernization and continued development

https://github.com/skydanc3r/aeinfo-modern