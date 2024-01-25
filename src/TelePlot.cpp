#include <TelePlot.h>

#ifdef ARDUINO_XIAO_ESP32C3
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
#endif
void TelePlot::Plot(String label, int value)
{
    Serial.print(">" + label);
    Serial.println(value);
}
void TelePlot::Plot(String label, long value)
{
    Serial.print(">" + label);
    Serial.println(value);
}

void TelePlot::Plot(String label, double value)
{
    Serial.print(">" + label);
    Serial.println(value);
}

void TelePlot::Plot(String label, float value)
{
    Serial.print(">" + label);
    Serial.println(value);
}

void TelePlot::Plot(String label, unsigned int value)
{
    Serial.print(">" + label);
    Serial.println(value);
}

void TelePlot::Plot(String label, unsigned long value)
{
    Serial.print(">" + label);
    Serial.println(value);
}