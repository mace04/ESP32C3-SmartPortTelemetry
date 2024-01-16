#include <Settings.h>

SmartPortSettings Settings::GetSmartPortSettings()
{
    Preferences settingsPreferences;
    if(!settingsPreferences.begin(SPORT_STORAGE_SPACE, true))
        Serial.println("Error initialising NVS for Sensors");
    SmartPortSettings settings;
    settings.BaudRate = settingsPreferences.getInt("baudRate", SPORT_BAUD);
    settings.RefreshRate = settingsPreferences.getInt("refreshRate", SPORT_REFRESH_RATE);
    settingsPreferences.end();
    return settings;
}

SensorSettings Settings::GetSensorSettings()
{
    Preferences settingsPreferences;
    if(!settingsPreferences.begin(SENSORS_STORAGE_SPACE, true))
        Serial.println("Error initialising NVS for Sensors");
    SensorSettings settings;

    settings.EnableSensorCURR = settingsPreferences.getBool("enableCURR", false);
    settings.EnableSensorVFAS = settingsPreferences.getBool("enableVFAS", false);
    settings.EnableSensorA3 = settingsPreferences.getBool("enableA3", false);
    settings.EnableSensorA4 = settingsPreferences.getBool("enableA4", false);
    settings.EnableSensorFuel = settingsPreferences.getBool("enableFuel", false);
    settings.VSensorR1 = settingsPreferences.getInt("vSensorR1", R1);
    settings.VSensorR2 = settingsPreferences.getInt("vSensorR2", R2);
    settings.MaxCurrent = settingsPreferences.getInt("maxCurrent", MAX_CURRENT);
    settings.CurrentPrecision = settingsPreferences.getDouble("currPrecision", CURRENT_PRECISSION);
    settings.CalibrationCURR = settingsPreferences.getDouble("calibrationCURR", CALIBRATION_CURR);
    settings.CalibrationVFAS = settingsPreferences.getDouble("calibrationVFAS", CALIBRATION_VFAS);
    settings.CalibrationA3 = settingsPreferences.getDouble("calibrationA3", CALIBRATION_A3);
    settings.CalibrationA4 = settingsPreferences.getDouble("calibrationA4", CALIBRATION_A4);
    settings.CalibrationFuel = settingsPreferences.getDouble("calibrationFuel", CALIBRATION_FUEL);
    settingsPreferences.end();
    return settings;
}

void Settings::SetSensorSettings(SensorSettings settings)
{
    Preferences settingsPreferences;
    if(!settingsPreferences.begin(SENSORS_STORAGE_SPACE, false))
        Serial.println("Error initialising NVS for Sensors");

	settingsPreferences.putBool("enableCURR", settings.EnableSensorCURR);
    settingsPreferences.putBool("enableVFAS", settings.EnableSensorVFAS);
    settingsPreferences.putBool("enableA3", settings.EnableSensorA3);
    settingsPreferences.putBool("enableA4", settings.EnableSensorA4);
    settingsPreferences.putBool("enableFuel", settings.EnableSensorFuel);
    settingsPreferences.putInt("vSensorR1", settings.VSensorR1);
    settingsPreferences.putInt("vSensorR2", settings.VSensorR2);
    settingsPreferences.putInt("maxCurrent", settings.MaxCurrent);
    settingsPreferences.putDouble("currPrecision", settings.CurrentPrecision);
    settingsPreferences.putDouble("calibrationCURR", settings.CalibrationCURR);
    settingsPreferences.putDouble("calibrationVFAS", settings.CalibrationVFAS);
    settingsPreferences.putDouble("calibrationA3", settings.CalibrationA3);
    settingsPreferences.putDouble("calibrationA4", settings.CalibrationA4);
    settingsPreferences.putDouble("calibrationFuel", settings.CalibrationFuel);  
    
    settingsPreferences.end();
  
}

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