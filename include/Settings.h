#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#ifdef ARDUINO_XIAO_ESP32C3
    #include <nvs_flash.h>
    #include <Preferences.h>
#else
    #include <EEPROM.h>
    #include <Streaming.h>
#endif

// Sensor Default Configuration
#define R1                      47000.0000  // Replaced by VPP
#define R2                      10000.0000  // Replaced by VPP
#define MAX_CURRENT             184.0   // Replaced by APP
#define CURRENT_PRECISSION		1.00  // Replaced by APP
#define CALIBRATION_CURR        1.00  // Replaced by VPP
#define CALIBRATION_VFAS        4.48  // Replaced by VPP
#define CALIBRATION_A3          4.48  // Replaced by VPP
#define CALIBRATION_A4          4.48  // Replaced by VPP
#define CALIBRATION_FUEL        8.8
#define MILLIVOLTS_PER_POINT    26.295454   // Default for max 36.3V @ 3.3V (4095 points)
#define MILLIAMPS_PER_POINT     24.42     // Default for max 100A @ 3.3V (4095 points)

// SmartPort Default Configuration
#define SPORT_BAUD              57600
#define SPORT_REFRESH_RATE      200

// WiFi Default Configuration
#define WIFI_SSID               "SKYPGFYX"
#define WIFI_PASSWORD           "HDXvtRKbPi8i"

#ifdef ARDUINO_XIAO_ESP32C3
#define SPORT_STORAGE_SPACE     "sport-config"
#define SENSORS_STORAGE_SPACE   "sensors-config"
#define WIFI_STORAGE_SPACE      "wifi-config"
#endif

struct SmartPortSettings
{
    unsigned int BaudRate;
    unsigned int RefreshRate;
};

struct SensorSettings
{
    bool EnableSensorCURR;
    bool EnableSensorVFAS;
    bool EnableSensorA3;
    bool EnableSensorA4;
    bool EnableSensorFuel;
    double AmpsPerPoint;    // In milliAmps
    double VoltsPerPoint;   // In milliVolts
};

#ifdef ARDUINO_XIAO_ESP32C3
struct WiFiSettings
{
    String WiFiSSID;
    String WiFiPassword;
    String HotspotSSID;
    String HotspotPassword;
};
#else
struct ArduinoSettings
{
    SmartPortSettings smartPortSettings;
    SensorSettings sensorSettings;
};
#endif

class Settings
{
    public:
        static SmartPortSettings GetSmartPortSettings();
        static void SetSmartPortSettings(SmartPortSettings settings);
        static SensorSettings GetSensorSettings();
        static void SetSensorSettings(SensorSettings settings);
#ifdef ARDUINO_XIAO_ESP32C3
        static WiFiSettings GetWiFiSettings();
        static void SetWiFiSettings(WiFiSettings settings);
#else
        static void handle();
#endif
    private:
#ifndef ARDUINO_XIAO_ESP32C3
        static ArduinoSettings ReadEeprom();
        static void WriteEeprom(ArduinoSettings settings);
        static String getValue(String data, char separator, int index);
#endif
        
};

#endif