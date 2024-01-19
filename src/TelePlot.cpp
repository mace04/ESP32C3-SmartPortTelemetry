#include <TelePlot.h>

void TelePlot::Plot(HardwareSerial serial, String label, int value)
{
    serial.print(">" + label);
    serial.println(value);
}
void TelePlot::Plot(HardwareSerial serial, String label, long value)
{
    serial.print(">" + label);
    serial.println(value);
}

void TelePlot::Plot(HardwareSerial serial, String label, double value)
{
    serial.print(">" + label);
    serial.println(value);
}

void TelePlot::Plot(HardwareSerial serial, String label, float value)
{
    serial.print(">" + label);
    serial.println(value);
}

void TelePlot::Plot(HardwareSerial serial, String label, unsigned int value)
{
    serial.print(">" + label);
    serial.println(value);
}

void TelePlot::Plot(HardwareSerial serial, String label, unsigned long value)
{
    serial.print(">" + label);
    serial.println(value);
}

void TelePlot::Plot(BluetoothSerial serial, String label, int value)
{
    serial.print(">" + label);
    serial.println(value);
}
void TelePlot::Plot(BluetoothSerial serial, String label, long value)
{
    serial.print(">" + label);
    serial.println(value);
}

void TelePlot::Plot(BluetoothSerial serial, String label, double value)
{
    serial.print(">" + label);
    serial.println(value);
}

void TelePlot::Plot(BluetoothSerial serial, String label, float value)
{
    serial.print(">" + label);
    serial.println(value);
}

void TelePlot::Plot(BluetoothSerial serial, String label, unsigned int value)
{
    serial.print(">" + label);
    serial.println(value);
}

void TelePlot::Plot(BluetoothSerial serial, String label, unsigned long value)
{
    serial.print(">" + label);
    serial.println(value);
}

