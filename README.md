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
