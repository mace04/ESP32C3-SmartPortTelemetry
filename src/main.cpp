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
#ifdef ARDUINO_XIAO_ESP32C3
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#endif
#include <Settings.h>
#include <SmartPort.h>

void handle_OnGet();
void handle_OnPost();
void handle_NotFound();
String SendHTML();


#define SERIAL_BAUD 115200

#ifdef ARDUINO_XIAO_ESP32C3
WebServer server(80);
#endif
SmartPort smartPort;
bool wifiConnected = true;
// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUD);

  // nvs_flash_erase(); // erase the NVS partition and...
  // nvs_flash_init(); // initialize the NVS partition.
  // while(true);

#ifdef ARDUINO_XIAO_ESP32C3
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

    Serial.println("Starting up Management Web Server");
    server.on("/", HTTPMethod::HTTP_GET, handle_OnGet);
    server.on("/", HTTPMethod::HTTP_POST, handle_OnPost);
    server.onNotFound(handle_NotFound);
    server.begin();
    Serial.println("Management Web Server started");
  }
#endif
  // Register sensors
  smartPort.Begin();

  // Serial.println("Initialis NVS");
  // nvs_flash_erase(); // erase the NVS partition and...
  // nvs_flash_init(); // initialize the NVS partition.
    // Preferences smartportSettings;
    // Preferences sensorSettings;
    // smartportSettings.begin(SPORT_STORAGE_SPACE, false);
    // sensorSettings.begin(SENSORS_STORAGE_SPACE, false);
  // Get baseline readings
  Serial.println("Initialise Completed...");
}

long int timer;
void loop() { 
  timer = millis();
#ifdef ARDUINO_XIAO_ESP32C3
  if(wifiConnected)
  {
    ArduinoOTA.handle();
    server.handleClient();
  }
#else
  Settings::handle();
#endif
  // put your main code here, to run repeatedly:
  smartPort.Hanlde();
}

#ifdef ARDUINO_XIAO_ESP32C3
void handle_OnGet() {
 
//  Temperature = dht.readTemperature(); // Gets the values of the temperature
//   Humidity = dht.readHumidity(); // Gets the values of the humidity 
  server.send(200, "text/html", SendHTML()); 
}

void handle_OnPost() {
  //TODO Set ALT settings
  SensorSettings sensorSettings;

  sensorSettings.EnableSensorCURR = server.arg("isSensorCurr") == "checked" ? true : false;
  sensorSettings.EnableSensorVFAS = server.arg("isSensorVFAS") == "checked" ? true : false;
  sensorSettings.EnableSensorA3 = server.arg("isSensorA3") == "checked" ? true : false;
  sensorSettings.EnableSensorA4 = server.arg("isSensorA4") == "checked" ? true : false;
  sensorSettings.EnableSensorFuel = server.arg("isSensorFuel") == "checked" ? true : false;
  sensorSettings.VoltsPerPoint = server.arg("voltsPerPoint").toFloat();
//   sensorSettings.AmpsPerPoint = server.arg("ampsPerPoint").toFloat();
  sensorSettings.CurrVoltageRef = server.arg("currVoltageRef").toFloat();
  sensorSettings.CurrSensitivity = server.arg("currSensitivity").toFloat();
  sensorSettings.CurrOffset = server.arg("currOffset").toFloat();

  Settings::SetSensorSettings(sensorSettings);

  WiFiSettings wifiSettings;
  wifiSettings.WiFiSSID = server.arg("wifiSSID");
  wifiSettings.WiFiPassword = server.arg("wifiPassword");
  wifiSettings.HotspotSSID = server.arg("hotspotSSID");
  wifiSettings.HotspotPassword = server.arg("hotspotPassword");
  Settings::SetWiFiSettings(wifiSettings);

  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML()
{
  //TODO Display ALT sensor and settings
  char ptr1[8];
  char ptr2[6];
  SensorSettings sensors = Settings::GetSensorSettings();
  SmartPortSettings sport = Settings::GetSmartPortSettings();
  WiFiSettings wifi = Settings::GetWiFiSettings();
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ESP32 Webserver</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: left; background-color: deepskyblue;}\n";
    ptr += "h1 {color: #444444;margin: 50px auto 30px; text-align: center}\n";
    ptr += "h2 {text-align: center}\n";
    ptr += "h3 {text-align: center}\n";
    ptr += "input{background-color:lightgrey;}\n";
    ptr += "input[type=text]{width: 85%;}\n";
    ptr += "input[type=password]{width: 85%;}\n";
    ptr += "table {width: 100%;}\n";
    ptr += "input[type=submit]{background-color:darkblue; width:100%; color: beige; text-align: center}\n";
    ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px; font-weight: bold;}\n";
    ptr += "label {font-size: 12px}";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESP32 FrSky Telemetry Hub</h1>\n";
  // ptr += "<h2>Sensor Settings</h2>\n";
  ptr += "<h2>" +  String(WiFi.getHostname()) + "</h3>\n";

  ptr += "<form action=""/"" method=""post"">\n";
      ptr += "<p>WiFi Settings</p>\n";
      ptr += "<table>\n";
          ptr += "<tr>\n";
            ptr += "<th><label for=""wifiSSID"">WiFi SSID </label><br><input type=""text"" id=""wifiSSID"" name=""wifiSSID"" value="" ";
              ptr += wifi.WiFiSSID;
              ptr += """></th>\n";          
            ptr += "<th><label for=""wifiPassword"">Password </label><br><input type=""password"" id=""wifiPassword"" name=""wifiPassword"" value="" ";
              ptr += wifi.WiFiPassword;
              ptr += """></th>\n";          
          ptr += "</tr>\n";
          ptr += "<tr>\n";
            ptr += "<th><label for=""hotspotSSID"">Hotspot SSID </label><br><input type=""text"" id=""hotspotSSID"" name=""hotspotSSID"" value="" ";
              ptr += wifi.HotspotSSID;
              ptr += """></th>\n";          
            ptr += "<th><label for=""hotspotPassword"">Password </label><br><input type=""password"" id=""hotspotPassword"" name=""hotspotPassword"" value="" ";
              ptr += wifi.HotspotPassword;
              ptr += """></th>\n";          
          ptr += "</tr>\n";
      ptr += "</table>\n";
      ptr += "<p>Active Sensors</p>\n";
      ptr += "<table>\n";
          ptr += "<tr>\n";
              ptr += "<th><input type=""checkbox"" id=""isSensorCurr"" name=""isSensorCurr"" ";
                ptr += sensors.EnableSensorCURR ? "CHECKED ": " ";
                ptr += "value=""checked""><label for=""isSensorCurr"">Current (Curr) </label></th>\n";
              ptr += "<th><input type=""checkbox"" id=""isSensorVFAS"" name=""isSensorVFAS"" ";
                ptr += sensors.EnableSensorVFAS ? "CHECKED ": " ";
                ptr += "value=""checked""><label for=""isSensorVFAS"">Battery Pack (VFAS) </label></th>\n";
          ptr += "</tr>\n";
          ptr += "<tr>\n";
              ptr += "<th><input type=""checkbox"" id=""isSensorFuel"" name=""isSensorFuel"" ";
                ptr += sensors.EnableSensorFuel ? "CHECKED ": " ";
                ptr += "value=""checked""><label for=""isSensorFuel"">Consumption (Fuel) </label></th>\n";
              ptr += "<th><input type=""checkbox"" id=""isSensorA3"" name=""isSensorA3"" ";
                ptr += sensors.EnableSensorA3 ? "CHECKED ": " ";
                ptr += "value=""checked""><label for=""isSensorA3"">Battery (A3) </label></th>\n";
          ptr += "</tr>\n";
          ptr += "<tr>\n";
              ptr += "<th><input type=""checkbox"" id=""isSensorA4"" name=""isSensorA4"" ";
                ptr += sensors.EnableSensorA4 ? "CHECKED ": " ";
                ptr += "value=""checked""><label for=""isSensorA4"">Battery (A4) </label></th>\n";
          ptr += "</tr>\n";
      ptr += "</table>\n";
      ptr += "<p>Sensors Setup</p>\n";
      ptr += "<table>\n";
          ptr += "<tr>\n";
                ptr += "<th>";
                    ptr += "<b>Voltage Sensor</b>";
                    ptr += "<br>\n";
                    ptr += "<label for=""voltsPerPoint"">mVolts/point </label><br><input type=""text"" id=""voltsPerPoint"" name=""voltsPerPoint"" value=""";
                    dtostrf(sensors.VoltsPerPoint,8, 4, ptr1);
                    ptr += String(ptr1);
                    ptr += """>";
                    ptr += "<br>\n";
                    ptr += "<br>\n";
                    ptr += "<br>\n";
                    ptr += "<br>\n";
                    ptr += "<br>\n";
                ptr += "</th>\n";
                ptr += "<th>";
                    ptr += "<b>Current Sensor</b>";
                    ptr += "<br>\n";
                    ptr += "<label for=""currVoltageRef"">Voltage Reference (V) </label><br><input type=""text"" id=""currVoltageRef"" name=""currVoltageRef"" value=""";
                    dtostrf(sensors.CurrVoltageRef,8, 4, ptr1);
                    ptr += String(ptr1);
                    ptr += """>";
                    ptr += "<br>\n";
                    ptr += "<label for=""currSensitivity"">Sensitivity (V/A) </label><br><input type=""text"" id=""currSensitivity"" name=""currSensitivity"" value=""";
                    dtostrf(sensors.CurrSensitivity,8, 4, ptr1);
                    ptr += String(ptr1);
                    ptr += """>";
                    ptr += "<br>\n";
                    ptr += "<label for=""currOffset"">Offset(V) </label><br><input type=""text"" id=""currOffset"" name=""currOffset"" value=""";
                    dtostrf(sensors.CurrOffset,8, 4, ptr1);
                    ptr += String(ptr1);
                    ptr += """>";
                ptr += "</th>\n";
          ptr += "</tr>\n";
          ptr += "<tr>\n";
            ptr += "<th>Max Voltage:";
              dtostrf(sensors.VoltsPerPoint / 1000.00 * 4095.00, 6, 2, ptr2);
              ptr += ptr2;
              ptr += "V</th>";
            ptr += "<th>Max Amps:";
              dtostrf((sensors.CurrVoltageRef - sensors.CurrOffset) / sensors.CurrSensitivity, 6, 2, ptr2);
              ptr += ptr2;
              ptr += "A</th>";
          ptr += "</tr>\n";
      ptr += "</table>\n";
      ptr += "<input type=""submit"" value=""Save"">\n";
  ptr += "</form>\n";
    
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";

  return ptr;

}
#endif