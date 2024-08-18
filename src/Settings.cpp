#include <Settings.h>

SmartPortSettings Settings::GetSmartPortSettings()
{
    SmartPortSettings settings;
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
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
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
   Preferences settingsPreferences;
    if(!settingsPreferences.begin(SENSORS_STORAGE_SPACE, true))
        Serial.println("Error initialising NVS for Sensors");

    settings.EnableSensorCURR = settingsPreferences.getBool("enableCURR", false);
    settings.EnableSensorVFAS = settingsPreferences.getBool("enableVFAS", false);
    settings.EnableSensorA3 = settingsPreferences.getBool("enableA3", false);
    settings.EnableSensorA4 = settingsPreferences.getBool("enableA4", false);
    settings.EnableSensorFuel = settingsPreferences.getBool("enableFuel", false);
    // settings.AmpsPerPoint = settingsPreferences.getDouble("mapp", MILLIAMPS_PER_POINT);
    settings.CurrVoltageRef = settingsPreferences.getDouble("vref", CURR_VOLT_REF);
    settings.CurrSensitivity = settingsPreferences.getDouble("mvpa", CURR_SENSITIVITY);
    settings.CurrOffset = settingsPreferences.getDouble("offset", CURR_OFFSET);
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
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
    Preferences settingsPreferences;
    if(!settingsPreferences.begin(SENSORS_STORAGE_SPACE, false))
        Serial.println("Error initialising NVS for Sensors");

	settingsPreferences.putBool("enableCURR", settings.EnableSensorCURR);
    settingsPreferences.putBool("enableVFAS", settings.EnableSensorVFAS);
    settingsPreferences.putBool("enableA3", settings.EnableSensorA3);
    settingsPreferences.putBool("enableA4", settings.EnableSensorA4);
    settingsPreferences.putBool("enableFuel", settings.EnableSensorFuel);
    // settingsPreferences.putDouble("mapp", settings.AmpsPerPoint);
    settingsPreferences.putDouble("vref", settings.CurrVoltageRef);
    settingsPreferences.putDouble("mvpa", settings.CurrSensitivity);
    settingsPreferences.putDouble("offset", settings.CurrOffset);
    settingsPreferences.putDouble("mvpp", settings.VoltsPerPoint);
    
    settingsPreferences.end();
#else
    ArduinoSettings fullSettings = ReadEeprom();
    fullSettings.sensorSettings = settings;
    WriteEeprom(fullSettings);
#endif
}

#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
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
        // settings.sensorSettings.AmpsPerPoint = MILLIAMPS_PER_POINT;
        settings.sensorSettings.CurrVoltageRef = CURR_VOLT_REF;
        settings.sensorSettings.CurrSensitivity = CURR_SENSITIVITY;
        settings.sensorSettings.CurrOffset = CURR_OFFSET;
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

void Settings::handle()
{
    if(Serial.available())
    {
        // Serial.setTimeout(50);
        String command = Serial.readStringUntil('\n');
        Serial << ":" << command << ":" << endl;
        if(command.startsWith("GET"))
        {
            ArduinoSettings settings = ReadEeprom();
            Serial << "SMARTPORT," << settings.smartPortSettings.BaudRate << "," << settings.smartPortSettings.RefreshRate << endl;
            Serial << "SENSORS," << settings.sensorSettings.EnableSensorCURR << "," 
                << settings.sensorSettings.EnableSensorVFAS << ","
                << settings.sensorSettings.EnableSensorFuel << ","
                << settings.sensorSettings.EnableSensorA3 << ","
                << settings.sensorSettings.EnableSensorA4 << ","
                // << _FLOAT(settings.sensorSettings.AmpsPerPoint, 6) << ","
                << _FLOAT(settings.sensorSettings.CurrVoltageRef, 6) << ","
                << _FLOAT(settings.sensorSettings.CurrSensitivity, 6) << ","
                << _FLOAT(settings.sensorSettings.CurrOffset, 6) << ","
                << _FLOAT(settings.sensorSettings.VoltsPerPoint, 6) << endl;
        }
        else if(command.startsWith("SET,SMARTPORT"))
        {
            ArduinoSettings settings = ReadEeprom();
            settings.smartPortSettings.BaudRate = getValue(command, ',', 2).toInt();
            settings.smartPortSettings.RefreshRate = getValue(command, ',', 3).toInt();
            WriteEeprom(settings);
            Serial << "SMARTPORT," << settings.smartPortSettings.BaudRate << "," << settings.smartPortSettings.RefreshRate << endl;
        }
        else if(command.startsWith("SET,SENSORS"))
        {
            ArduinoSettings settings = ReadEeprom();
            settings.sensorSettings.EnableSensorCURR = getValue(command, ',', 2).toInt() == 1 ? true : false;
            settings.sensorSettings.EnableSensorVFAS = getValue(command, ',', 3).toInt() == 1 ? true : false;
            settings.sensorSettings.EnableSensorFuel = getValue(command, ',', 4).toInt() == 1 ? true : false;
            settings.sensorSettings.EnableSensorA3 = getValue(command, ',', 5).toInt() == 1 ? true : false;
            settings.sensorSettings.EnableSensorA4 = getValue(command, ',', 6).toInt() == 1 ? true : false;
            // settings.sensorSettings.AmpsPerPoint = getValue(command, ',', 7).toFloat();
            settings.sensorSettings.CurrVoltageRef = getValue(command, ',', 7).toFloat();
            settings.sensorSettings.CurrSensitivity = getValue(command, ',', 8).toFloat();
            settings.sensorSettings.CurrOffset = getValue(command, ',', 9).toFloat();
            settings.sensorSettings.VoltsPerPoint = getValue(command, ',', 10).toFloat();
            WriteEeprom(settings);
            Serial << "SENSORS," << settings.sensorSettings.EnableSensorCURR << "," 
                << settings.sensorSettings.EnableSensorVFAS << ","
                << settings.sensorSettings.EnableSensorFuel << ","
                << settings.sensorSettings.EnableSensorA3 << ","
                << settings.sensorSettings.EnableSensorA4 << ","
                // << _FLOAT(settings.sensorSettings.AmpsPerPoint, 6) << ","
                << _FLOAT(settings.sensorSettings.CurrVoltageRef, 6) << ","
                << _FLOAT(settings.sensorSettings.CurrSensitivity, 6) << ","
                << _FLOAT(settings.sensorSettings.CurrOffset, 6) << ","
                << _FLOAT(settings.sensorSettings.VoltsPerPoint, 6) << endl;        
        }
        else if(command.startsWith("RESET"))
        {
            EEPROM.write(0x00, 0xFF);
            Serial << "RESET,OK" << endl;
        }
        
    }
}

String Settings::getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
#endif