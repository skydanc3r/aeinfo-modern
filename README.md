# aeinfo-modern

Modernized version of **aeinfo 2.4** for the Aqua Computer Aquaero 4.

This project is based on the original work by Christian Unger (2006–2007) and aims to make the software usable on modern Linux systems while extending it with new features.

## Current status

- ✅ Debian 13 compatible
- ✅ GCC 14 compatible
- ✅ Exit segmentation fault fixed
- ✅ USB communication working
- ✅ aquaerod builds successfully

## Planned

- ESP32 USB Host implementation
- Modern Web Interface
- REST API
- MQTT support
- Home Assistant integration
- FHEM module
- Improved documentation

## Project goals

The long-term goal is to replace the original Windows software with a modern, platform-independent solution.

The ESP32 will communicate directly with the Aquaero 4 via USB and provide:

- Web interface
- REST API
- MQTT (optional)
- Remote configuration
- Monitoring

## Credits

Original aeinfo 2.4 by Christian Unger.

This repository contains a modernized version intended to preserve and extend support for the Aquaero 4.