# SmartPort Telemetry Hub

This is an ESP32C3 SmartPort Telemetry Hub project designed to interface with FrSky RC telemetry systems. It acts as a sensor hub that sends telemetry data to FrSky receivers using the SmartPort protocol.

## Core Functionality
- **FrSky SmartPort Telemetry Provider**: Acts as a sensor hub that sends telemetry data to FrSky receivers using the SmartPort protocol at 57600 baud
- **Multi-Platform Support**: Compatible with ESP32C3, ESP32, Arduino Uno, and Arduino Nano boards

## Supported Telemetry Sensors
- **VFAS (Battery Voltage)**: Measures battery pack voltage with configurable scaling
- **CURR (Current)**: Measures current draw using analog sensors with calibration for voltage reference, sensitivity, and offset
- **FUEL (Power Consumption)**: Calculates cumulative power consumption based on current readings
- **A3/A4 (Analog Voltages)**: Additional voltage measurement channels
- **ALT (Altitude)**: Barometric altitude using BMP180 sensor with baseline pressure calibration
- **GPS Data** (placeholders): Longitude/latitude, GPS altitude, GPS speed
- **Air Speed** (placeholder): Airspeed measurement capability

## Hardware Features
- **Analog Sensor Reading**: Multi-sample averaging with configurable sample rates and moving average filtering
- **BMP180 Integration**: I2C-connected barometric pressure sensor for altitude measurements
- **Configurable Pin Mapping**: All sensor pins and SmartPort RX/TX pins are configurable
- **Signal Inversion**: Optional inverted serial communication for SmartPort

## Configuration & Management
- **Persistent Settings Storage**:
  - ESP32 variants use NVS (Non-Volatile Storage)
  - Arduino variants use EEPROM
- **Web-Based Configuration** (ESP32 only): Complete web interface served via AsyncWebServer with SPIFFS for static files
- **Serial Command Interface** (Arduino variants): Text-based commands for configuration
- **WiFi Management** (ESP32 only): Configurable WiFi connection with fallback to hotspot mode
- **OTA Updates** (ESP32 only): Over-the-air firmware updates

## Web Interface Features (ESP32)
- Real-time configuration of all sensors and settings
- WiFi network setup and hotspot configuration
- Sensor enable/disable and pin assignment
- Calibration parameters for voltage and current sensors
- SmartPort communication settings (pins, refresh rate, signal inversion)
- Dynamic display of calculated maximum voltage/current ranges

## Advanced Capabilities
- **Sensor Calibration**: Comprehensive calibration system for voltage scaling and current sensor parameters
- **Data Filtering**: Moving average smoothing to reduce sensor noise
- **Power Calculation**: Real-time fuel consumption tracking with configurable calibration factor
- **Polling-Based Transmission**: Responds to SmartPort receiver polls with appropriate sensor data
- **Multi-Sensor Coordination**: Manages multiple sensors with independent refresh rates and data formats

## Development & Build
- **PlatformIO Build System**: Multi-environment configuration for different target boards
- **Modular Architecture**: Separate classes for sensors, SmartPort communication, and settings management
- **Library Dependencies**: SparkFun BMP180, ESP Async WebServer, Streaming (for Arduino)

This project transforms microcontroller boards into dedicated telemetry sensors for RC aircraft, providing comprehensive flight data to FrSky-compatible transmitters and receivers.

## Known issues
**High Impact**
- Reverse signal option cannot be enabled: the checkbox posts the string "checked" and toInt() yields 0, plus Inverted is never persisted or loaded on ESP32, so the UI setting is effectively ignored. See index.html:133, main.cpp:411-414, Settings.cpp:10-31.
- A3/A4 telemetry scaling is inconsistent: values are sent without the *100 scaling used elsewhere, which likely breaks SmartPort centi‑unit expectations. See SmartPort.cpp:251-259 vs SmartPort.cpp:155-158.
- Settings changes from the web UI will not update sensor registration or cached sensor settings, because only SmartPort settings are refreshed in the loop. See SmartPort.cpp:218-221 and cached sensor settings in Sensors.cpp:22-41.
- WiFi connection flow treats empty SSID as “connected”, which starts OTA/SPIFFS/web server even when there is no network. See main.cpp:56-95.

**Medium/Low Impact**
- Fuel/consumption integration divides by elapsed time, which makes consumption larger at shorter loop intervals; typical integration should multiply by time. See Sensors.cpp:281-286.
- Voltage smoothing mixes VFAS/A3/A4 readings because one moving‑average buffer is shared for all voltage channels. See Sensors.cpp:10-15 and Sensors.cpp:275-276.
- ESP32 SmartPort pins default to 0 on first boot because getUInt() uses no default; the struct’s defaults never get applied. See Settings.cpp:10-13.
- dtostrf buffers are too small for width 8 + null terminator, risking overflow. See main.cpp:236-258.
- Merge conflict markers remain in the README. See README.md:53-57.
- WiFi credentials are hard‑coded in the header, which is risky if this repo is shared. See Settings.h:30-32.