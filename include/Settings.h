#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <nvs_flash.h>
#include <Preferences.h>

// Sensor Default Configuration
#define R1                      47000.0000
#define R2                      10000.0000
#define MAX_CURRENT             184.0
#define CURRENT_PRECISSION		1.00
#define CALIBRATION_CURR        1.00
#define CALIBRATION_VFAS        4.48
#define CALIBRATION_A3          4.48
#define CALIBRATION_A4          4.48
#define CALIBRATION_FUEL        8.8

// SmartPort Default Configuration
#define SPORT_BAUD              57600
#define SPORT_REFRESH_RATE      300

// WiFi Default Configuration
#define WIFI_SSID               "SKYPGFYX"
#define WIFI_PASSWORD           "HDXvtRKbPi8i"

#define SPORT_STORAGE_SPACE     "sport-config"
#define SENSORS_STORAGE_SPACE   "sensors-config"
#define WIFI_STORAGE_SPACE      "wifi-config"

struct SmartPortSettings
{
    int BaudRate;
    int RefreshRate;
};

struct SensorSettings
{
    bool EnableSensorCURR;
    bool EnableSensorVFAS;
    bool EnableSensorA3;
    bool EnableSensorA4;
    bool EnableSensorFuel;
    int VSensorR1;
    int VSensorR2;
    int MaxCurrent;
    float CurrentPrecision;
    float CalibrationCURR;
    float CalibrationVFAS;
    float CalibrationA3;
    float CalibrationA4;
    float CalibrationFuel;
};

struct WiFiSettings
{
    String WiFiSSID;
    String WiFiPassword;
    String HotspotSSID;
    String HotspotPassword;
};

class Settings
{
    public:
        static SmartPortSettings GetSmartPortSettings();
        void SetSmartPortSettings(SmartPortSettings settings);
        static SensorSettings GetSensorSettings();
        static void SetSensorSettings(SensorSettings settings);
        static WiFiSettings GetWiFiSettings();
        static void SetWiFiSettings(WiFiSettings settings);
};

#endif