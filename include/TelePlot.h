#ifndef TELEPLOT_H
#define TELEPLOT_H

#include <Arduino.h>
#include "BluetoothSerial.h"

class TelePlot
{
    public:
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
};
#endif