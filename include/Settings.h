#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
    #include <nvs_flash.h>
    #include <Preferences.h>
#else
    #include <EEPROM.h>
    // #include <Streaming.h>
#endif

// Sensor Default Configuration
#define MAX_CURRENT             184.0   // Replaced by APP
#define CALIBRATION_FUEL        8.8
#define CURR_VOLT_REF           5.00     // Voltage Reference of current sensor
#define CURR_SENSITIVITY        40.00    // mV/A sensitivity of current sensor
#define CURR_OFFSET             2.54    // Zero-current output sensor voltage (offset voltage)
#define MILLIVOLTS_PER_POINT    26.295454   // Default for max 36.3V @ 3.3V (4095 points)
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
    #define ADC_MAX_VALUE 4095.00
#else   
    #define ADC_MAX_VALUE 1023.00
#endif

// SmartPort Default Configuration
#define SPORT_BAUD              57600
#define SPORT_REFRESH_RATE      200

// WiFi Default Configuration
#define WIFI_SSID               "SKYPGFYX"
#define WIFI_PASSWORD           "HDXvtRKbPi8i"

#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
#define SPORT_STORAGE_SPACE     "sport-config"
#define SENSORS_STORAGE_SPACE   "sensors-config"
#define WIFI_STORAGE_SPACE      "wifi-config"
#endif

struct SmartPortSettings
{
    unsigned int BaudRate = SPORT_BAUD; // Not to be changed. Always use default
    unsigned int RefreshRate = SPORT_REFRESH_RATE;
    uint8_t RxPin = 13; //Defaulted for Arduino
    uint8_t TxPin = 13; //Defaulted for Arduino
    bool Inverted = false;
};

struct SensorSettings
{
    bool EnableSensorCURR = false;
    bool EnableSensorVFAS = false;
    bool EnableSensorA3 = false;
    bool EnableSensorA4 = false;
    bool EnableSensorFuel = false;
    uint8_t CurrSensorPin;
    uint8_t VfasSensorPin;
    uint8_t A3SensorPin;
    uint8_t A4SensorPin;
    // double AmpsPerPoint;    // In milliAmps
    double CurrVoltageRef;
    double CurrSensitivity;
    double CurrOffset;
    double VoltsPerPoint;   // In milliVolts
};

#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
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
        static void Reset();
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
        static WiFiSettings GetWiFiSettings();
        static void SetWiFiSettings(WiFiSettings settings);
#else
        static void handle(const String& command);
#endif
    private:
#if !defined(ARDUINO_XIAO_ESP32C3) && !defined(ESP32)
        static ArduinoSettings ReadEeprom();
        static void WriteEeprom(ArduinoSettings settings);
        static void PrintSettings();
        static void SetSettingByName(const String& settingName, const String& value);
        static String getValue(String data, char separator, int index);
#endif
        
};

#endif