#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <Settings.h>

#define PIN_CURR          A0	//Current Sensor - Range 0-3.3V
#define PIN_VFAS          A1	//VFAS Sensor - Range 0-3.3V - Battery Range 0-18V
#define PIN_A3            A2	//A3 Sensor - Range 0-3.3V - Battery Range 0-18V
#ifndef ARDUINO_XIAO_ESP32C3
  #define PIN_A4          A3	//A4 Sensor - Range 0-3.3V - Battery Range 0-18V#
#endif

#define SENSOR_ALT            0x0100
#define SENSOR_CURR           0x0200
#define SENSOR_VFAS           0x0210
#define SENSOR_FUEL           0x0600
#define SENSOR_GPS_LONG_LATI  0x0800
#define SENSOR_GPS_ALT        0x0820
#define SENSOR_GPS_SPEED      0x0830
#define SENSOR_A3             0x0900
#define SENSOR_A4             0x0910
#define SENSOR_AIR_SPEED      0x0a00

#define SAMPLE_RATES			30
#define SAMPLE_DELAY_MS   5


class Sensors
{
  public:
    Sensors();
    void Begin();
    void RegisterSensors();
    bool IsRegistered(uint16_t sensorId);
    float ReadSensor(uint16_t sensorId);

  private:
    float baselinePressure;
    SFE_BMP180 bmp180;              // BMP180 sensor
    int voltageTestValue = 0;
    unsigned long timer;
    float current;
    float consumption;
    SensorSettings settings;

    bool isSensorAlt = false;
    bool isSensorCurr = false;
    bool isSensorVfas = false;
    bool isSensorFuel = false;
    bool isSensorGpsPos = false;
    bool isSensorGpsAlt = false;
    bool isSensorGpsSpd = false;
    bool isSensorA3 = false;
    bool isSensorA4 = false;
    bool isSensorSpd = false;

    double GetPressure();
    //TODO Create method to read ALT sensor
    float GetCurrent();
    float GetVoltage(int pin);
    float GetPowerConsumption();
    void SetPowerConsumption();

};

#endif