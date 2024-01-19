#ifndef TELEPLOT_H
#define TELEPLOT_H

#include <Arduino.h>
// #include <SoftwareSerial.h>
#include "BluetoothSerial.h"

typedef enum SerialType {HWSerial, SWSerial, BTSerial};
class TelePlot
{
    //TODO Consider converting the methods to static
    private:
        SerialType serialType;
        HardwareSerial hwSerial;
        // SoftwareSerial swSerial;
        BluetoothSerial btSerial;
        void PrintLabel(String label);
        void PrintValue(int value);
        void PrintValue(long value);
        void PrintValue(double value);
        void PrintValue(float value);
        void PrintValue(unsigned int value);
        void PrintValue(unsigned long value);

    public:
        TelePlot(HardwareSerial serial);
        TelePlot(BluetoothSerial serial);
        // TelePlot(SoftwareSerial serial);
        void Plot(String name, int value);
        void Plot(String name, double value);
        void Plot(String name, float value);
        void Plot(String name, long value);
        void Plot(String name, unsigned int value);
        void Plot(String name, unsigned long value);        
};
#endif