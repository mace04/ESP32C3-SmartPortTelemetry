#ifndef TELEPLOT_H
#define TELEPLOT_H

#include <Arduino.h>
#ifdef ARDUINO_XIAO_ESP32C3
#include "BluetoothSerial.h"
#endif
class TelePlot
{
    public:
#ifdef ARDUINO_XIAO_ESP32C3
        static void Plot(HardwareSerial serial, String label, int value);
        static void Plot(HardwareSerial serial, String label, long value);
        static void Plot(HardwareSerial serial, String label, double value);
        static void Plot(HardwareSerial serial, String label, float value);
        static void Plot(HardwareSerial serial, String label, unsigned int value);
        static void Plot(HardwareSerial serial, String label, unsigned long value);
        static void Plot(BluetoothSerial serial, String label, int value);
        static void Plot(BluetoothSerial serial, String label, long value);
        static void Plot(BluetoothSerial serial, String label, double value);
        static void Plot(BluetoothSerial serial, String label, float value);
        static void Plot(BluetoothSerial serial, String label, unsigned int value);
        static void Plot(BluetoothSerial serial, String label, unsigned long value);      
#endif
        static void Plot(String label, int value);
        static void Plot(String label, long value);
        static void Plot(String label, double value);
        static void Plot(String label, float value);
        static void Plot(String label, unsigned int value);
        static void Plot(String label, unsigned long value);
};
#endif