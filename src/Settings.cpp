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

void Settings::handle(const String& command)
{
    if(command.length() > 0)
    {
        // Serial.setTimeout(50);
        if(getValue(command, ' ', 0).equalsIgnoreCase("GET"))
        {
            PrintSettings();
        }
        else if(getValue(command, ' ', 0).equalsIgnoreCase("SET") )
        {
            SetSettingByName(getValue(command, ' ', 1), getValue(command, ' ', 2));
        }
        else if(getValue(command, ' ', 0).equalsIgnoreCase("RESET"))
        {
            Reset();
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

void Settings::PrintSettings()
{
    SensorSettings sensor = GetSensorSettings();
    SmartPortSettings sp = GetSmartPortSettings();

    Serial << F("{") << endl;
    Serial << F("\t\"sensors\":") << endl;
    Serial << F("\t{") << endl;
    Serial << F("\t\t\"enableCURR\": ") << (sensor.EnableSensorCURR ? "true" : "false") << F(",") << endl;
    Serial << F("\t\t\"currSensorPin\": ") << sensor.CurrSensorPin  << F(",") << endl;
    Serial << F("\t\t\"enableVFAS\": ") << (sensor.EnableSensorVFAS ? "true" : "false") << F(",") << endl;
    Serial << F("\t\t\"vfasSensorPin\": ") << sensor.VfasSensorPin << F(",") << endl;
    Serial << F("\t\t\"enableFuel\": ") << (sensor.EnableSensorFuel ? "true" : "false") << F(",") << endl;
    Serial << F("\t\t\"enableA3\": ") << (sensor.EnableSensorA3 ? "true" : "false") << F(",") << endl;
    Serial << F("\t\t\"a3SensorPin\": ") << sensor.A3SensorPin << F(",") << endl;
    Serial << F("\t\t\"enableA4\": ") << (sensor.EnableSensorA4 ? "true" : "false") << F(",") << endl;
    Serial << F("\t\t\"a4SensorPin\": ") << sensor.A4SensorPin << F(",") << endl;
    Serial << F("\t\t\"voltsPerPoint\": ") << sensor.VoltsPerPoint << F(",") << endl;
    Serial << F("\t\t\"currVoltageRef\": ") << sensor.CurrVoltageRef << F(",") << endl;
    Serial << F("\t\t\"currSensitivity\": ") << sensor.CurrSensitivity << F(",") << endl;
    Serial << F("\t\t\"currOffset\": ") << sensor.CurrOffset << endl;
    Serial << F("\t},") << endl;

    Serial << F("\t\"smartport\":") << endl;
    Serial << F("\t{") << endl;
    Serial << F("\t\t\"baudRate\": ") << sp.BaudRate << F(",") << endl;
    Serial << F("\t\t\"rxPin\": ") << sp.RxPin << F(",") << endl;
    Serial << F("\t\t\"txPin\": ") << sp.TxPin << F(",") << endl;
    Serial << F("\t\t\"refreshRate\": ") << sp.RefreshRate << F(",") << endl;
    Serial << F("\t\t\"inverted\": ") << (sp.Inverted ? "true" : "false") << endl;
    Serial << F("\t},") << endl;

    // Serial.println(F("=== MAX Values ==="));
    Serial << F("\t\"maxVoltage\": ") << (sensor.VoltsPerPoint / 1000.00 * ADC_MAX_VALUE) << F(",") << endl;
    Serial << F("\t\"maxCurrent\": ") << ((sensor.CurrVoltageRef - sensor.CurrOffset) / (sensor.CurrSensitivity / 1000.00)) << endl;
    Serial << F("}") << endl;
    Serial.flush();
}

void Settings::SetSettingByName(const String& settingName, const String& value)
{
    bool sensorChanged = false;
    bool spChanged = false;

    SensorSettings sensor = GetSensorSettings();
    SmartPortSettings sp = GetSmartPortSettings();

    // SensorSettings
    if (settingName.equalsIgnoreCase("enableCURR")) {
        sensor.EnableSensorCURR = (value == "1" || value.equalsIgnoreCase("true"));
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("currSensorPin")) {
        sensor.CurrSensorPin = value.toInt();
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("enableVFAS")) {
        sensor.EnableSensorVFAS = (value == "1" || value.equalsIgnoreCase("true"));
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("vfasSensorPin")) {
        sensor.VfasSensorPin = value.toInt();
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("enableFuel")) {
        sensor.EnableSensorFuel = (value == "1" || value.equalsIgnoreCase("true"));
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("enableA3")) {
        sensor.EnableSensorA3 = (value == "1" || value.equalsIgnoreCase("true"));
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("a3SensorPin")) {
        sensor.A3SensorPin = value.toInt();
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("enableA4")) {
        sensor.EnableSensorA4 = (value == "1" || value.equalsIgnoreCase("true"));
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("a4SensorPin")) {
        sensor.A4SensorPin = value.toInt();
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("voltsPerPoint")) {
        sensor.VoltsPerPoint = value.toFloat();
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("currVoltageRef")) {
        sensor.CurrVoltageRef = value.toFloat();
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("currSensitivity")) {
        sensor.CurrSensitivity = value.toFloat();
        sensorChanged = true;
    } else if (settingName.equalsIgnoreCase("currOffset")) {
        sensor.CurrOffset = value.toFloat();
        sensorChanged = true;
    }
    // SmartPortSettings
    else if (settingName.equalsIgnoreCase("baudRate")) {
        sp.BaudRate = value.toInt();
        spChanged = true;
    } else if (settingName.equalsIgnoreCase("rxPin")) {
        sp.RxPin = value.toInt();
        spChanged = true;
    } else if (settingName.equalsIgnoreCase("txPin")) {
        sp.TxPin = value.toInt();
        spChanged = true;
    } else if (settingName.equalsIgnoreCase("refreshRate")) {
        sp.RefreshRate = value.toInt();
        spChanged = true;
    } else if (settingName.equalsIgnoreCase("inverted")) {
        sp.Inverted = (value == "1" || value.equalsIgnoreCase("true"));
        spChanged = true;
    }

    if (sensorChanged) {
        SetSensorSettings(sensor);
    }
    if (spChanged) {
        SetSmartPortSettings(sp);
    }
    // Serial << F("{ \"success\": true }") << endl;
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
    Serial.println(F("RESET,OK"));
#endif
}