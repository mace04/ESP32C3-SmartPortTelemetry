#include <Settings.h>

SmartPortSettings Settings::GetSmartPortSettings()
{
    SmartPortSettings settings;
 #ifdef ARDUINO_XIAO_ESP32C3
    Preferences settingsPreferences;
    if(!settingsPreferences.begin(SPORT_STORAGE_SPACE, true))
        Serial.println("Error initialising NVS for Sensors");
    settings.BaudRate = settingsPreferences.getInt("baudRate", SPORT_BAUD);
    settings.RefreshRate = settingsPreferences.getInt("refreshRate", SPORT_REFRESH_RATE);
    settingsPreferences.end();
#else
    settings = ReadEeprom().smartPortSettings;
#endif
    return settings;
}

SensorSettings Settings::GetSensorSettings() 
{
    SensorSettings settings;
    //TODO Add ALT sensor
    //TODO Add calibration settings for ALT sensor
 #ifdef ARDUINO_XIAO_ESP32C3
   Preferences settingsPreferences;
    if(!settingsPreferences.begin(SENSORS_STORAGE_SPACE, true))
        Serial.println("Error initialising NVS for Sensors");

    settings.EnableSensorCURR = settingsPreferences.getBool("enableCURR", false);
    settings.EnableSensorVFAS = settingsPreferences.getBool("enableVFAS", false);
    settings.EnableSensorA3 = settingsPreferences.getBool("enableA3", false);
    settings.EnableSensorA4 = settingsPreferences.getBool("enableA4", false);
    settings.EnableSensorFuel = settingsPreferences.getBool("enableFuel", false);
    settings.AmpsPerPoint = settingsPreferences.getDouble("mapp", MILLIAMPS_PER_POINT);
    settings.VoltsPerPoint = settingsPreferences.getDouble("mvpp", MILLIVOLTS_PER_POINT);

    settingsPreferences.end();
#else
    settings = ReadEeprom().sensorSettings;
#endif
    return settings;
}

void Settings::SetSensorSettings(SensorSettings settings)
{
    //TODO Enable/disable ALT sensor
    //TODO Set calibration settings for ALT sensor
    //TODO Save sensor settings to EEPROM for Arduino
 #ifdef ARDUINO_XIAO_ESP32C3
    Preferences settingsPreferences;
    if(!settingsPreferences.begin(SENSORS_STORAGE_SPACE, false))
        Serial.println("Error initialising NVS for Sensors");

	settingsPreferences.putBool("enableCURR", settings.EnableSensorCURR);
    settingsPreferences.putBool("enableVFAS", settings.EnableSensorVFAS);
    settingsPreferences.putBool("enableA3", settings.EnableSensorA3);
    settingsPreferences.putBool("enableA4", settings.EnableSensorA4);
    settingsPreferences.putBool("enableFuel", settings.EnableSensorFuel);
    settingsPreferences.putDouble("mapp", settings.AmpsPerPoint);
    settingsPreferences.putDouble("mvpp", settings.VoltsPerPoint);
    
    settingsPreferences.end();
#else
    ArduinoSettings fullSettings = ReadEeprom();
    fullSettings.sensorSettings = settings;
    WriteEeprom(fullSettings);
#endif
}

#ifdef ARDUINO_XIAO_ESP32C3
WiFiSettings Settings::GetWiFiSettings()
{
    Preferences settingsPreferences;
    if(!settingsPreferences.begin(WIFI_STORAGE_SPACE, true))
        Serial.println("Error initialising NVS for WiFi");
    WiFiSettings settings;

    settings.WiFiSSID = settingsPreferences.getString("wifiSSID", String(WIFI_SSID));
    settings.WiFiPassword = settingsPreferences.getString("wifiPassword", String(WIFI_PASSWORD));
    settings.HotspotSSID = settingsPreferences.getString("hotspotSSID", "");
    settings.HotspotPassword = settingsPreferences.getString("hotspotPassword", "");
    settingsPreferences.end();
    return settings;
}

void Settings::SetWiFiSettings(WiFiSettings settings)
{
    Preferences settingsPreferences;
    if(!settingsPreferences.begin(WIFI_STORAGE_SPACE, false))
        Serial.println("Error initialising NVS for WiFi");
	
    settingsPreferences.putString("wifiSSID", settings.WiFiSSID);
    settingsPreferences.putString("wifiPassword", settings.WiFiPassword);
    settingsPreferences.putString("hotspotSSID", settings.HotspotSSID);
    settingsPreferences.putString("hotspotPassword", settings.HotspotPassword);
    settingsPreferences.end();
}
#else
ArduinoSettings Settings::ReadEeprom()
{
    ArduinoSettings settings;

    if (EEPROM.read(0x00) == 0xFF) {
        // Not saved data so use default data
        settings.smartPortSettings.BaudRate = SPORT_BAUD;
        settings.smartPortSettings.RefreshRate = SPORT_REFRESH_RATE;

        settings.sensorSettings.EnableSensorCURR = false;
        settings.sensorSettings.EnableSensorVFAS = true;
        settings.sensorSettings.EnableSensorA3 = false;
        settings.sensorSettings.EnableSensorA4 = false;
        settings.sensorSettings.EnableSensorFuel = settings.sensorSettings.EnableSensorCURR;
        settings.sensorSettings.AmpsPerPoint = MILLIAMPS_PER_POINT;
        settings.sensorSettings.VoltsPerPoint = MILLIVOLTS_PER_POINT;
    }
    else 
    {
        EEPROM.get(0x00, settings);
    }
    return settings;
}

void Settings::WriteEeprom(ArduinoSettings settings)
{
    EEPROM.put(0x00, settings);
}
#endif