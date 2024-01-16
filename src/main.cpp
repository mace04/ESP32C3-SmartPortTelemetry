#include <Arduino.h>
#include <SmartPort.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Settings.h>

void handle_OnGet();
void handle_OnPost();
void handle_NotFound();
String SendHTML();


#define SERIAL_BAUD 115200

WebServer server(80);
SmartPort smartPort;
bool wifiConnected = true;
// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUD);

  // nvs_flash_erase(); // erase the NVS partition and...
  // nvs_flash_init(); // initialize the NVS partition.
  // while(true);

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

void loop() {
  if(wifiConnected)
  {
    ArduinoOTA.handle();
    server.handleClient();
  }

  // put your main code here, to run repeatedly:
  smartPort.Hanlde();
}

void handle_OnGet() {
 
//  Temperature = dht.readTemperature(); // Gets the values of the temperature
//   Humidity = dht.readHumidity(); // Gets the values of the humidity 
  server.send(200, "text/html", SendHTML()); 
}

void handle_OnPost() {
  SensorSettings sensorSettings;

  sensorSettings.EnableSensorCURR = server.arg("isSensorCurr") == "checked" ? true : false;
  sensorSettings.EnableSensorVFAS = server.arg("isSensorVFAS") == "checked" ? true : false;
  sensorSettings.EnableSensorA3 = server.arg("isSensorA3") == "checked" ? true : false;
  sensorSettings.EnableSensorA4 = server.arg("isSensorA4") == "checked" ? true : false;
  sensorSettings.EnableSensorFuel = server.arg("isSensorFuel") == "checked" ? true : false;
  sensorSettings.VSensorR1 = server.arg("resistor1").toInt();
  sensorSettings.VSensorR2 = server.arg("resistor2").toInt();
  sensorSettings.MaxCurrent = server.arg("maxCurrent").toInt();
  sensorSettings.CurrentPrecision = server.arg("currentPresition").toFloat();
  sensorSettings.CalibrationCURR = server.arg("currentCalibration").toFloat();
  sensorSettings.CalibrationVFAS = server.arg("vfasCalibration").toFloat();
  sensorSettings.CalibrationA3 = server.arg("a3Calibration").toFloat();
  sensorSettings.CalibrationA4 = server.arg("a4Calibration").toFloat();
  sensorSettings.CalibrationFuel = server.arg("fuelCalibration").toFloat();
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
  char ptr1[6];
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
              ptr += "<th><label for=""resistor1"">R1 </label><br><input type=""text"" id=""resistor1"" name=""resistor1"" value="" ";
                ptr += String(sensors.VSensorR1);
                ptr += """></th>\n";
              ptr += "<th><label for=""resistor2"">R2 </label><br><input type=""text"" id=""resistor2"" name=""resistor2"" value="" ";
                ptr += String(sensors.VSensorR2);
                ptr += """></th>\n";
          ptr += "</tr>\n";
          ptr += "<tr>\n";
              ptr += "<th><label for=""maxCurrent"">Maximum Current </label><br><input type=""text"" id=""maxCurrent"" name=""maxCurrent"" value="" ";
                ptr += String(sensors.MaxCurrent);
                ptr += """></th>\n";
          ptr += "</tr>\n";
      ptr += "</table>\n";
      ptr += "<p>Sensor Calibration</p>\n";
      ptr += "<table>\n";
          ptr += "<tr>\n";
              ptr += "<th><label for=""currentPresition"">Current Precision </label><br><input type=""text"" id=""currentPresition"" name=""currentPresition"" value=""";
                dtostrf(sensors.CurrentPrecision,6, 2, ptr1);
                ptr += String(ptr1);
                ptr += """></th>\n";
              ptr += "<th><label for=""currentCalibration"">Current Calibration Value </label><br><input type=""text"" id=""currentCalibration"" name=""currentCalibration"" value=""";
                dtostrf(sensors.CalibrationCURR,6, 2, ptr1);
                ptr += String(ptr1);
                ptr += """></th>\n";
          ptr += "</tr>\n";
          ptr += "<tr>\n";
              ptr += "<th><label for=""vfasCalibration"">VFAS Calibration Value </label><br><input type=""text"" id=""vfasCalibration"" name=""vfasCalibration"" value=""";
                dtostrf(sensors.CalibrationVFAS,6, 2, ptr1);
                ptr += String(ptr1);
                ptr += """></th>\n";
              ptr += "<th><label for=""a3Calibration"">A3 Calibration Value </label><br><input type=""text"" id=""a3Calibration"" name=""a3Calibration"" value=""";
                dtostrf(sensors.CalibrationA3,6, 2, ptr1);
                ptr += String(ptr1);
                ptr += """></th>\n";
          ptr += "</tr>\n";
          ptr += "<tr>\n";
              ptr += "<th><label for=""a4Calibration"">A4 Calibration Value </label><br><input type=""text"" id=""a4Calibration"" name=""a4Calibration"" value=""";
                dtostrf(sensors.CalibrationA4,6, 2, ptr1);
                ptr += String(ptr1);
                ptr += """></th>\n";
              ptr += "<th><label for=""fuelCalibration"">Fuel Calibration Value </label><br><input type=""text"" id=""fuelCalibration"" name=""fuelCalibration"" value=""";
                dtostrf(sensors.CalibrationFuel,6, 2, ptr1);
                // Serial.printf("Fuel Calibration: %.2f\n", sensors.CalibrationFuel);
                ptr += String(ptr1);
                ptr += """></th>\n";
          ptr += "</tr>\n";
      ptr += "</table>\n";
      ptr += "<input type=""submit"" value=""Save"">\n";
  ptr += "</form>\n";
    
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";

  return ptr;

}