#include <Settings.h>

SmartPortSettings Settings::GetSmartPortSettings()
{
    SmartPortSettings settings;
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
    Preferences settingsPreferences;
    if(!settingsPreferences.begin(SPORT_STORAGE_SPACE, true))
        Serial.println("Error initialising NVS for SmartPort Telemetry");
    settings.BaudRate = settingsPreferences.getUInt("baudRate", SPORT_BAUD);
    settings.RefreshRate = settingsPreferences.getUInt("refreshRate", SPORT_REFRESH_RATE);
    settings.RxPin = settingsPreferences.getUInt("rxPin");
    settings.TxPin = settingsPreferences.getUInt("txPin");
    settingsPreferences.end();
#else
    settings = ReadEeprom().smartPortSettings;
#endif
    return settings;
}

void Settings::SetSmartPortSettings(SmartPortSettings settings)
{
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
    Preferences settingsPreferences;
    if(!settingsPreferences.begin(SPORT_STORAGE_SPACE, false))
        Serial.println("Error initialising NVS for SmartPort Telemetry");
	
    settingsPreferences.putUInt("baudRate", settings.BaudRate);
    settingsPreferences.putUInt("refreshRate", settings.RefreshRate);
    settingsPreferences.putUInt("rxPin", settings.RxPin);
    settingsPreferences.putUInt("txPin", settings.TxPin);
    settingsPreferences.end();
#else
    ArduinoSettings fullSettings = ReadEeprom();
    fullSettings.smartPortSettings = settings;
    WriteEeprom(fullSettings);
#endif
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
    settings.CurrSensorPin = settingsPreferences.getUInt("CurrSensorPin");
    settings.VfasSensorPin = settingsPreferences.getUInt("VfasSensorPin");
    settings.A3SensorPin = settingsPreferences.getUInt("A3SensorPin");
    settings.A4SensorPin = settingsPreferences.getUInt("A4SensorPin");
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
    settingsPreferences.putUInt("CurrSensorPin", settings.CurrSensorPin);
    settingsPreferences.putUInt("VfasSensorPin", settings.VfasSensorPin);
    settingsPreferences.putUInt("A3SensorPin", settings.A3SensorPin);
    settingsPreferences.putUInt("A4SensorPin", settings.A4SensorPin);
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
        settings.sensorSettings.CurrSensorPin = 0;
        settings.sensorSettings.VfasSensorPin = 0;
        settings.sensorSettings.A3SensorPin = 0;
        settings.sensorSettings.A4SensorPin = 0;
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
                << settings.sensorSettings.CurrSensorPin << ","
                << settings.sensorSettings.EnableSensorVFAS << ","
                << settings.sensorSettings.VfasSensorPin << ","
                << settings.sensorSettings.EnableSensorFuel << ","
                << settings.sensorSettings.EnableSensorA3 << ","
                << settings.sensorSettings.A3SensorPin << ","
                << settings.sensorSettings.EnableSensorA4 << ","
                << settings.sensorSettings.A4SensorPin << ","
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
            settings.sensorSettings.CurrSensorPin = getValue(command, ',', 3).toInt();
            settings.sensorSettings.EnableSensorVFAS = getValue(command, ',', 4).toInt() == 1 ? true : false;
            settings.sensorSettings.VfasSensorPin = getValue(command, ',', 5).toInt();
            settings.sensorSettings.EnableSensorFuel = getValue(command, ',', 6).toInt() == 1 ? true : false;
            settings.sensorSettings.EnableSensorA3 = getValue(command, ',', 7).toInt() == 1 ? true : false;
            settings.sensorSettings.A3SensorPin = getValue(command, ',', 8).toInt();
            settings.sensorSettings.EnableSensorA4 = getValue(command, ',', 9).toInt() == 1 ? true : false;
            settings.sensorSettings.A4SensorPin = getValue(command, ',', 10).toInt();
            settings.sensorSettings.CurrVoltageRef = getValue(command, ',', 11).toFloat();
            settings.sensorSettings.CurrSensitivity = getValue(command, ',', 12).toFloat();
            settings.sensorSettings.CurrOffset = getValue(command, ',', 13).toFloat();
            settings.sensorSettings.VoltsPerPoint = getValue(command, ',', 14).toFloat();
            WriteEeprom(settings);
            Serial << "SENSORS," << settings.sensorSettings.EnableSensorCURR << "," 
                << settings.sensorSettings.CurrSensorPin << ","
                << settings.sensorSettings.EnableSensorVFAS << ","
                << settings.sensorSettings.VfasSensorPin << ","
                << settings.sensorSettings.EnableSensorFuel << ","
                << settings.sensorSettings.EnableSensorA3 << ","
                << settings.sensorSettings.A3SensorPin << ","
                << settings.sensorSettings.EnableSensorA4 << ","
                << settings.sensorSettings.A4SensorPin << ","
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

void Settings::Reset()
{
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
    Preferences settingsPreferences;
    settingsPreferences.begin(WIFI_STORAGE_SPACE, false);
    settingsPreferences.clear();
    settingsPreferences.end();
    settingsPreferences.begin(SPORT_STORAGE_SPACE, false);
    settingsPreferences.clear();
    settingsPreferences.end();
    settingsPreferences.begin(SENSORS_STORAGE_SPACE, false);
    settingsPreferences.clear();
    settingsPreferences.end();
#else
    EEPROM.write(0x00, 0xFF);
    Serial << "RESET,OK" << endl;
#endif
}