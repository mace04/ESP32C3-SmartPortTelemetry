/*
Custom Smartport sensor to provide telementry for:
  * VFAS
  * CURR
  * ALT
  * A3
  * A4

Reference to other projects and information:
  * https://www.rcgroups.com/forums/showthread.php?2245978-FrSky-S-Port-telemetry-library-easy-to-use-and-configurable 
  * https://www.rcgroups.com/forums/showthread.php?2465555-FrSky-(old)-telemetry-library-easy-to-use-and-configurable 
  * https://www.rcgroups.com/forums/showthread.php?2245978-FrSky-S-Port-telemetry-library-easy-to-use-and-configurable
  * https://code.google.com/archive/p/telemetry-convert/wikis/FrSkySPortProtocol.wiki
  * https://github.com/jcheger/frsky-arduino/blob/master/FrskySP/examples/FrskySP_sensor_demo/FrskySP_sensor_demo.ino
  * https://github.com/RealTadango/FrSky/tree/master/examples
  * https://wiki.seeedstudio.com/xiaoesp32c3-flash-storage/
*/

#include <Arduino.h>
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
    #include <WiFi.h>
    #include <ArduinoOTA.h>
    #include <ESPAsyncWebServer.h>
    #include <SPIFFS.h>
#endif
#include <Settings.h>
#include <SmartPort.h>

#define SERIAL_BAUD 38400

#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
    AsyncWebServer server(80);
    String httpParamsHandle(const String &param);
    void httpPostAction(AsyncWebServerRequest *request);
#else
    static String input = "";
    String ReadLineFromSerialNonBlocking();
#endif
SmartPort smartPort;
bool wifiConnected = true;


// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUD);
//   Serial.println(F("Initialising..."));

//   Serial.println("Reseting memory.");
//   Settings::Reset();

#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
  unsigned long wifiConnectionTime;
  WiFi.mode(WIFI_STA);
  WiFiSettings wifiSettings = Settings::GetWiFiSettings();
  if(!wifiSettings.WiFiSSID.isEmpty())
  {
    WiFi.begin(wifiSettings.WiFiSSID.c_str(), wifiSettings.WiFiPassword.c_str());
    Serial.print("Connecting to WiFi ..");
    wifiConnectionTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(500);
      if(millis() - wifiConnectionTime > 5000)
      {
        Serial.println();
        Serial.print("Unable to connect to WiFi");
        wifiConnected = false;
        break;
      }
    }
  }
  Serial.println();
  if (!wifiConnected && !String(wifiSettings.HotspotSSID).isEmpty())
  {
    WiFi.begin(wifiSettings.HotspotSSID.c_str(), wifiSettings.HotspotPassword.c_str());
    Serial.print("Connecting to Hotspot ..");
    wifiConnectionTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(500);
      if(millis() - wifiConnectionTime > 5000)
      {
        Serial.println();
        Serial.println("Unable to connect to Hotspot");
        wifiConnected = false;
        break;
      }
    }
    Serial.println();
  }

  if(wifiConnected)
  {
    Serial.printf("IP Address: %s\n\n", WiFi.localIP().toString());
    Serial.println("Initialising OTA...");
    ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("(OTA) Start updating " + type);
      })
      .onEnd([]() {
        Serial.println("\n(OTA) End");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("(OTA) Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("(OTA) Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
      });

    ArduinoOTA.begin();
    Serial.println("OTA Setup completed");
    Serial.println();

    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }    
    else
    {
        Serial.println("Filesystem mounted successfully.");
    }

    Serial.println("Starting up Management Web Server");
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(SPIFFS, "/index.html", String(), false, httpParamsHandle); });
    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(SPIFFS, "/style.css", "text/css"); });
    server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(SPIFFS, "/index.js", String()); });
    server.on("/", HTTP_POST, httpPostAction);
    server.begin();
    Serial.println("Management Web Server started");
  }
#endif
  // Register sensors
  smartPort.Begin();

//   Serial.println(F("Initialise Completed..."));
}

long int timer;
void loop() { 
  timer = millis();
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
  if(wifiConnected)
  {
    ArduinoOTA.handle();
  }
#else
  String command = ReadLineFromSerialNonBlocking();
  // Handle the command
  Settings::handle(command);
  command == "";
#endif
  // put your main code here, to run repeatedly:
  smartPort.Hanlde();
}

#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
String httpParamsHandle(const String &param)
{
    if (param == "hostname")
    {
        return String(WiFi.getHostname());
    }
    if (param == "wifiSSID")
    {
        return Settings::GetWiFiSettings().WiFiSSID;
    }
    if (param == "wifiPassword")
    {
        return Settings::GetWiFiSettings().WiFiPassword;
    }
    if (param == "hotspotSSID")
    {
        return Settings::GetWiFiSettings().HotspotSSID;
    }
    if (param == "hotspotPassword")
    {
        return Settings::GetWiFiSettings().HotspotPassword;
    }
    if (param == "isSensorCurr")
    {
        return Settings::GetSensorSettings().EnableSensorCURR ? "CHECKED" : "";
    }
    if (param == "currentSensorPin")
    {
        return String(Settings::GetSensorSettings().CurrSensorPin);
    }
    if (param == "isSensorVFAS")
    {
        return Settings::GetSensorSettings().EnableSensorVFAS ? "CHECKED" : "";
    }
    if (param == "vfasSensorPin")
    {
        return String(Settings::GetSensorSettings().VfasSensorPin);
    }
    if (param == "isSensorFuel")
    {
        return Settings::GetSensorSettings().EnableSensorFuel ? "CHECKED" : "";
    }
    if (param == "isSensorA3")
    {
        return Settings::GetSensorSettings().EnableSensorA3 ? "CHECKED" : "";
    }
    if (param == "a3SensorPin")
    {
        return String(Settings::GetSensorSettings().A3SensorPin);
    }
    if (param == "isSensorA4")
    {
        return Settings::GetSensorSettings().EnableSensorA4 ? "CHECKED" : "";
    }
    if (param == "a4SensorPin")
    {
        return String(Settings::GetSensorSettings().A4SensorPin);
    }
    if (param == "voltsPerPoint")
    {
        char ptr[8];
        dtostrf(Settings::GetSensorSettings().VoltsPerPoint, 8, 4, ptr);
        return String(ptr);
    }
    if (param == "currVoltageRef")
    {
        char ptr[8];
        dtostrf(Settings::GetSensorSettings().CurrVoltageRef, 8, 4, ptr);
        return String(ptr);
    }
    if (param == "currSensitivity")
    {
        char ptr[8];
        dtostrf(Settings::GetSensorSettings().CurrSensitivity, 8, 4, ptr);
        return String(ptr);
    }
    if (param == "currOffset")
    {
        char ptr[8];
        dtostrf(Settings::GetSensorSettings().CurrOffset, 8, 4, ptr);
        return String(ptr);
    }
    if (param == "maxVolts")
    {
        char ptr[8];
        dtostrf(Settings::GetSensorSettings().VoltsPerPoint / 1000.00 * 4095.00, 6, 2, ptr);
        return String(ptr);
    }
    if (param == "maxAmps")
    {
        char ptr[8];
        dtostrf((Settings::GetSensorSettings().CurrVoltageRef - Settings::GetSensorSettings().CurrOffset) / (Settings::GetSensorSettings().CurrSensitivity / 1000.00), 6, 2, ptr);
        return String(ptr);
    }
    if (param == "smartportRxPin")
    {
        Serial.printf("GET[%s]:%s\n", param.c_str(), String(Settings::GetSmartPortSettings().RxPin));
        return String(Settings::GetSmartPortSettings().RxPin);
    }
    if (param == "smartportTxPin")
    {
        Serial.printf("GET[%s]:%s\n", param.c_str(), String(Settings::GetSmartPortSettings().TxPin));
        return String(Settings::GetSmartPortSettings().TxPin);
    }
    if (param == "smartportRefreshRate")
    {
        Serial.printf("GET[%s]:%s\n", param.c_str(), String(Settings::GetSmartPortSettings().RefreshRate));
        return String(Settings::GetSmartPortSettings().RefreshRate);
    }
    if (param == "smartReverseSignal")
    {
        Serial.printf("GET[%s]:%s\n", param.c_str(), String(Settings::GetSmartPortSettings().Inverted));
        return Settings::GetSmartPortSettings().Inverted ? "CHECKED" : "";
    }
    return "";
}

void httpPostAction(AsyncWebServerRequest *request)
{
    WiFiSettings wifiSettings;
    bool isWifiSettings = false;
    SensorSettings sensorSettings;
    bool isSensorSettings = false;
    SmartPortSettings spSettings;
    bool isSmartportSettings = false;

    int params = request->params();
    for (int i = 0; i < params; i++)
    {
        AsyncWebParameter *p = request->getParam(i);
        Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());

        if (p->name() == "wifiSSID")
        {
            wifiSettings.WiFiSSID = p->value();
            isWifiSettings = true;
        }
        if (p->name() == "wifiPassword")
        {
            wifiSettings.WiFiPassword = p->value();
            isWifiSettings = true;
        }
        if (p->name() == "hotspotSSID")
        {
            wifiSettings.HotspotSSID = p->value();
            isWifiSettings = true;
        }
        if (p->name() == "hotspotPassword")
        {
            wifiSettings.HotspotPassword = p->value();
            isWifiSettings = true;
        }

        if (p->name() == "isSensorCurr")
        {
            sensorSettings.EnableSensorCURR = true;
            isSensorSettings = true;
        }
        if (p->name() == "currentSensorPin")
        {
            sensorSettings.CurrSensorPin = p->value().toInt();
            isSensorSettings = true;
        }
        if (p->name() == "isSensorVFAS")
        {
            sensorSettings.EnableSensorVFAS = true;
            isSensorSettings = true;
        }
        if (p->name() == "vfasSensorPin")
        {
            sensorSettings.VfasSensorPin = p->value().toInt();
            isSensorSettings = true;
        }
        if (p->name() == "isSensorFuel")
        {
            sensorSettings.EnableSensorFuel = true;
            isSensorSettings = true;
        }
        if (p->name() == "isSensorA3")
        {
            sensorSettings.EnableSensorA3 = true;
            isSensorSettings = true;
        }
        if (p->name() == "a3SensorPin")
        {
            sensorSettings.A3SensorPin = p->value().toInt();
            isSensorSettings = true;
        }
        if (p->name() == "isSensorA4")
        {
            sensorSettings.EnableSensorA4 = true;
            isSensorSettings = true;
        }
        if (p->name() == "a4SensorPin")
        {
            sensorSettings.A4SensorPin = p->value().toInt();
            isSensorSettings = true;
        }
        if (p->name() == "voltsPerPoint")
        {
            sensorSettings.VoltsPerPoint = p->value().toFloat();
            isSensorSettings = true;
        }
        if (p->name() == "currVoltageRef")
        {
            sensorSettings.CurrVoltageRef = p->value().toFloat();
            isSensorSettings = true;
        }
        if (p->name() == "currSensitivity")
        {
            sensorSettings.CurrSensitivity = p->value().toFloat();
            isSensorSettings = true;
        }
        if (p->name() == "currOffset")
        {
            sensorSettings.CurrOffset = p->value().toFloat();
            isSensorSettings = true;
        }
        if (p->name() == "smartportRxPin")
        {
            spSettings.RxPin = p->value().toInt();
            isSmartportSettings = true;
        }
        if (p->name() == "smartportTxPin")
        {
            spSettings.TxPin = p->value().toInt();
            isSmartportSettings = true;
        }
        if (p->name() == "smartportRefreshRate")
        {
            spSettings.RefreshRate = p->value().toInt();
            isSmartportSettings = true;
        }
        if (p->name() == "smartReverseSignal")
        {
            spSettings.Inverted = (p->value() == "1" || p->value().equalsIgnoreCase("true") || p->value().equalsIgnoreCase("checked"));
            isSmartportSettings = true;
        }

    }
    if (isSensorSettings)
        Settings::SetSensorSettings(sensorSettings);
    if (isWifiSettings)
        Settings::SetWiFiSettings(wifiSettings);
    if (isSmartportSettings)
        Settings::SetSmartPortSettings(spSettings);

    request->send(SPIFFS, "/index.html", String(), false, httpParamsHandle);
}
#else
// Non-blocking serial line reader for Settings class
// Call repeatedly in loop() or a task. Returns "" if line not complete.
String ReadLineFromSerialNonBlocking()
{
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (input.length() > 0) {
                String result = input;
                input = "";
                return result;
        }
        } else {
            input += c;
        }
    }
    return "";
}
#endif