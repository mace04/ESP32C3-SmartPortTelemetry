#ifndef SMARTPORT_H
#define SMARTPORT_H

#include <Arduino.h>
#include <Settings.h>
#include <Sensors.h>
#if  !defined(ARDUINO_XIAO_ESP32C3) && !defined(ESP32)
    #include <SoftwareSerial.h>
#endif

#define FRSKY_SENSORID_VARIO            0x00
#define FRSKY_SENSORID_FLVSS            0xA1
#define FRSKY_SENSORID_FAS              0x22
#define FRSKY_SENSORID_GPS              0x83
#define FRSKY_SENSORID_RPM              0xE4
#define FRSKY_SENSORID_SP2UART          0x45
#define FRSKY_SENSORID_FUEL             0x98

#define FRSKY_VALUE_TYPE_ALT            0x0100
#define FRSKY_VALUE_TYPE_CURR           0x0200
#define FRSKY_VALUE_TYPE_VFAS           0x0210
#define FRSKY_VALUE_TYPE_FUEL           0x0600
#define FRSKY_VALUE_TYPE_GPS_LONG_LATI  0x0800
#define FRSKY_VALUE_TYPE_GPS_ALT        0x0820
#define FRSKY_VALUE_TYPE_GPS_SPEED      0x0830
#define FRSKY_VALUE_TYPE_A3             0x0900
#define FRSKY_VALUE_TYPE_A4             0x0910
#define FRSKY_VALUE_TYPE_AIR_SPEED      0x0a00

typedef union {
    byte byteValue[4];
    long longValue = 0;
} SmartPortData;

struct SmartPortFrame
{
    bool isRegistered = false;
    uint8_t frameHeader = 0x10;
    uint16_t sensorId = 0;
    SmartPortData value;
    uint8_t crc = 0;
    unsigned long lastSent = 0;
};

class SmartPort
{
    public:
        SmartPort();
        void Begin(uint8_t rxPin = D7, uint8_t txPin = D6, bool inverted = false);
        bool IsRegistered(uint16_t sensorId);
        void Hanlde();
        void RefreshSettings(bool SmartPortRefresh, bool SensorRefresh);

    private:
        unsigned long sensorPolling0x22Loop = 0;
        int softwarePin = -1;
#if defined(ARDUINO_XIAO_ESP32C3) || defined(ESP32)
        HardwareSerial* smartPort = new HardwareSerial(1);
#else
        SoftwareSerial* smartPort;
#endif
        Sensors* sensors = new Sensors();
        SmartPortFrame altData;
        SmartPortFrame currData;
        SmartPortFrame vfasData;
        SmartPortFrame fuelData;
        SmartPortFrame gpsPosData;
        SmartPortFrame gpsAltData;
        SmartPortFrame gpsSpeedData;
        SmartPortFrame a3Data;
        SmartPortFrame a4Data;
        SmartPortFrame airSpeedData;
        SmartPortSettings settings;
        long int lastPacketSent;

        void RegisterSensors();
        void RegisterSensor(uint16_t sensorId);
        void ReadSensors();
        void SetSensorValue(uint16_t sensorId, uint32_t value);
        byte GetChecksum(SmartPortFrame data);
        void SendData(SmartPortFrame& data);
        void SendByte(byte b);
};

#endif