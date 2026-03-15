# TrailCurrent Plateau

Vehicle leveling module that monitors trailer signals and controls leveling via a CAN bus interface with OTA firmware update capability. Part of the [TrailCurrent](https://trailcurrent.com) open-source vehicle platform.

## Hardware Overview

- **Microcontroller:** ESP32-S3
- **IMU:** Adafruit BNO055 9-DOF Absolute Orientation
- **Function:** Vehicle leveling with CAN bus communication
- **Key Features:**
  - BNO055 IMU for pitch/roll measurement with auto-calibration
  - Per-corner height adjustment calculation from vehicle dimensions
  - Three mounting orientations (floor, left wall, right wall) with axis remapping
  - CAN bus communication at 500 kbps
  - Vehicle configuration (dimensions, mounting) received via CAN and stored in NVS
  - Over-the-air (OTA) firmware updates via WiFi (credentials provisioned via CAN)
  - RGB LED status indicator
  - Custom flash partition layout with dual OTA slots

## Hardware Requirements

### Components

- **Microcontroller:** [Waveshare ESP32-S3-Zero](https://www.waveshare.com/esp32-s3-zero.htm?aff_id=Trailcurrent) — selected for its extensive documentation, small footprint, pre-soldered programming pins, castellations for direct PCB integration, and low power consumption
- **CAN Transceiver:** SN65HVD230
- **Power:** Buck converter (12V step-down)
- **Connectors:** JST XH 2.54 4-pin

### Pin Connections

| GPIO | Function |
|------|----------|
| 21 | RGB status LED |
| 1 | I2C SDA (BNO055) |
| 2 | I2C SCL (BNO055) |
| 7 | CAN TX |
| 8 | CAN RX |

### KiCAD Library Dependencies

This project uses the consolidated [TrailCurrentKiCADLibraries](https://github.com/trailcurrentoss/TrailCurrentKiCADLibraries).

**Setup:**

```bash
# Clone the library
git clone git@github.com:trailcurrentoss/TrailCurrentKiCADLibraries.git

# Set environment variables (add to ~/.bashrc or ~/.zshrc)
export TRAILCURRENT_SYMBOL_DIR="/path/to/TrailCurrentKiCADLibraries/symbols"
export TRAILCURRENT_FOOTPRINT_DIR="/path/to/TrailCurrentKiCADLibraries/footprints"
export TRAILCURRENT_3DMODEL_DIR="/path/to/TrailCurrentKiCADLibraries/3d_models"
```

See [KICAD_ENVIRONMENT_SETUP.md](https://github.com/trailcurrentoss/TrailCurrentKiCADLibraries/blob/main/KICAD_ENVIRONMENT_SETUP.md) in the library repository for detailed setup instructions.

## Opening the Project

1. **Set up environment variables** (see Library Dependencies above)
2. **Open KiCAD:**
   ```bash
   kicad EDA/TrailCurrentPlateau/TrailCurrentPlateau.kicad_pro
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
pio run -t upload --upload-port esp32s3-DEVICE_ID
```

### Firmware Dependencies

This firmware depends on the following public libraries:

- **[ESP32ArduinoDebugLibrary](https://github.com/trailcurrentoss/ESP32ArduinoDebugLibrary)** - Debug macros
- **[OtaUpdateLibraryWROOM32](https://github.com/trailcurrentoss/OtaUpdateLibraryWROOM32)** - Over-the-air firmware update functionality
- **[TwaiTaskBasedLibraryWROOM32](https://github.com/trailcurrentoss/TwaiTaskBasedLibraryWROOM32)** - CAN bus communication interface
- **[Adafruit BNO055](https://github.com/adafruit/Adafruit_BNO055)** (v1.6.3) - IMU sensor driver
- **[Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)** (v1.1.14) - Sensor abstraction layer

All dependencies are automatically resolved by PlatformIO during the build process.

**WiFi Credentials (for OTA updates):**
- WiFi credentials are provisioned over the CAN bus (ID `0x01`) and stored in NVS
- No hardcoded credentials or secrets files needed

### CAN Bus Protocol

- **Receive ID `0x00`:** OTA update trigger — initiates firmware update when device hostname matches
- **Receive ID `0x01`:** WiFi credential provisioning — multi-frame protocol to store SSID/password in NVS
- **Receive ID `0x20`:** Leveling configuration — set mounting surface, vehicle length/width, persist to NVS
- **Transmit ID `0x30`:** Tilt data — pitch/roll angles (×100) and front-back/left-right height diffs in mm
- **Transmit ID `0x31`:** Corner data — per-corner height adjustments in mm (normalized, lowest = 0)
- **Transmit ID `0x32`:** Status data — IMU connection, calibration levels, mounting orientation
- CAN speed: 500 kbps

## Manufacturing

- **PCB Files:** Ready for fabrication via standard PCB services (JLCPCB, OSH Park, etc.)
- **BOM Generation:** Export BOM from KiCAD schematic (Tools > Generate BOM)
- **JLCPCB Assembly:** See [BOM_ASSEMBLY_WORKFLOW.md](https://github.com/trailcurrentoss/TrailCurrentKiCADLibraries/blob/main/BOM_ASSEMBLY_WORKFLOW.md) for detailed assembly workflow

## Project Structure

```
├── EDA/                          # KiCAD hardware design files
│   └── TrailCurrentPlateau/
│       ├── *.kicad_pro           # KiCAD project
│       ├── *.kicad_sch           # Schematic
│       └── *.kicad_pcb           # PCB layout
├── src/                          # Firmware source
│   ├── Main.cpp                  # Main application
│   ├── Globals.h                 # Debug macros and data structures
│   └── wifiConfig.h              # CAN-based WiFi credential provisioning
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
- **KiCAD setup:** See [KICAD_ENVIRONMENT_SETUP.md](https://github.com/trailcurrentoss/TrailCurrentKiCADLibraries/blob/main/KICAD_ENVIRONMENT_SETUP.md)
- **Assembly workflow:** See [BOM_ASSEMBLY_WORKFLOW.md](https://github.com/trailcurrentoss/TrailCurrentKiCADLibraries/blob/main/BOM_ASSEMBLY_WORKFLOW.md)
