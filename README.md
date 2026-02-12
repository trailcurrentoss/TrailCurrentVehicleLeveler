# TrailCurrent Vehicle Leveler

Vehicle leveling module that monitors trailer signals and controls leveling via a CAN bus interface with OTA firmware update capability.

## Hardware Overview

- **Microcontroller:** ESP32-C6
- **Function:** Vehicle leveling control with CAN bus communication
- **Key Features:**
  - CAN bus communication at 500 kbps
  - Over-the-air (OTA) firmware updates via WiFi
  - RGB LED status indicator
  - Trailer signal monitoring (lights, brakes, turn signals)
  - Custom flash partition layout with dual OTA slots

## Hardware Requirements

### Components

- **Microcontroller:** ESP32-C6-Super-Mini
- **CAN Transceiver:** SN65HVD230
- **Power:** Buck converter (12V step-down)
- **Connectors:** JST XH 2.54 4-pin

### Pin Connections

| GPIO | Function |
|------|----------|
| 8 | RGB status LED |
| 15 | CAN RX |
| 14 | CAN TX |

### KiCAD Library Dependencies

This project uses the consolidated [TrailCurrentKiCADLibraries](https://codeberg.org/trailcurrentopensource/TrailCurrentKiCADLibraries).

**Setup:**

```bash
# Clone the library
git clone https://codeberg.org/trailcurrentopensource/TrailCurrentKiCADLibraries.git

# Set environment variables (add to ~/.bashrc or ~/.zshrc)
export TRAILCURRENT_SYMBOL_DIR="/path/to/TrailCurrentKiCADLibraries/symbols"
export TRAILCURRENT_FOOTPRINT_DIR="/path/to/TrailCurrentKiCADLibraries/footprints"
export TRAILCURRENT_3DMODEL_DIR="/path/to/TrailCurrentKiCADLibraries/3d_models"
```

See [KICAD_ENVIRONMENT_SETUP.md](https://codeberg.org/trailcurrentopensource/TrailCurrentKiCADLibraries/blob/main/KICAD_ENVIRONMENT_SETUP.md) in the library repository for detailed setup instructions.

## Opening the Project

1. **Set up environment variables** (see Library Dependencies above)
2. **Open KiCAD:**
   ```bash
   kicad EDA/TrailCurrentVehicleLeveler/TrailCurrentVehicleLeveler.kicad_pro
   ```
3. **Verify libraries load** - All symbol and footprint libraries should resolve without errors
4. **View 3D models** - Open PCB and press `Alt+3` to view the 3D visualization

## Firmware

See `src/` directory for PlatformIO-based firmware.

**Setup:**
```bash
# Install PlatformIO (if not already installed)
pip install platformio

# Build firmware
pio run

# Upload to board (serial)
pio run -t upload

# Upload via OTA (after initial flash)
pio run -t upload --upload-port esp32c6-DEVICE_ID
```

### Firmware Dependencies

This firmware depends on the following public libraries:

- **[C6SuperMiniRgbLedLibrary](https://codeberg.org/trailcurrentopensource/C6SuperMiniRgbLedLibrary)** (v0.0.1) - RGB LED status indicator driver
- **[Esp32C6OtaUpdateLibrary](https://codeberg.org/trailcurrentopensource/Esp32C6OtaUpdateLibrary)** (v0.0.1) - Over-the-air firmware update functionality
- **[Esp32C6TwaiTaskBasedLibrary](https://codeberg.org/trailcurrentopensource/Esp32C6TwaiTaskBasedLibrary)** (v0.0.3) - CAN bus communication interface

All dependencies are automatically resolved by PlatformIO during the build process.

**WiFi Credentials (for OTA updates):**
- Copy `src/Secrets.h.example` to `src/Secrets.h` and fill in your WiFi credentials
- Never commit `Secrets.h` to version control (it's in `.gitignore`)

### CAN Bus Protocol

- **Receive ID `0x00`:** OTA update notifications - triggers firmware update when device ID matches
- CAN speed: 500 kbps

### Trailer Signal Monitoring

The firmware tracks:
- Tail/running light status
- Left/right turn and brake status
- Reverse status
- Electric brake status
- 12V power reading

## Manufacturing

- **PCB Files:** Ready for fabrication via standard PCB services (JLCPCB, OSH Park, etc.)
- **BOM Generation:** Export BOM from KiCAD schematic (Tools > Generate BOM)
- **JLCPCB Assembly:** See [BOM_ASSEMBLY_WORKFLOW.md](https://codeberg.org/trailcurrentopensource/TrailCurrentKiCADLibraries/blob/main/BOM_ASSEMBLY_WORKFLOW.md) for detailed assembly workflow

## Project Structure

```
├── EDA/                          # KiCAD hardware design files
│   └── TrailCurrentVehicleLeveler/
│       ├── *.kicad_pro           # KiCAD project
│       ├── *.kicad_sch           # Schematic
│       └── *.kicad_pcb           # PCB layout
├── src/                          # Firmware source
│   ├── Main.cpp                  # Main application
│   ├── Globals.h                 # Debug macros and data structures
│   └── Secrets.h.example         # WiFi credentials template
├── platformio.ini                # Build configuration
└── partitions.csv                # ESP32 flash partition layout
```

## License

MIT License - See LICENSE file for details.

This is open source hardware. You are free to use, modify, and distribute these designs for personal or commercial purposes.

## Contributing

Improvements and contributions are welcome! Please submit issues or pull requests.

## Support

For questions about:
- **KiCAD setup:** See [KICAD_ENVIRONMENT_SETUP.md](https://codeberg.org/trailcurrentopensource/TrailCurrentKiCADLibraries/blob/main/KICAD_ENVIRONMENT_SETUP.md)
- **Assembly workflow:** See [BOM_ASSEMBLY_WORKFLOW.md](https://codeberg.org/trailcurrentopensource/TrailCurrentKiCADLibraries/blob/main/BOM_ASSEMBLY_WORKFLOW.md)
