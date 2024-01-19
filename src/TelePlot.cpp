#include <TelePlot.h>

TelePlot::TelePlot(HardwareSerial serial)
{
    hwSerial = serial;
    serialType = SerialType::HWSerial;
}
TelePlot::TelePlot(BluetoothSerial serial)
{
    btSerial = serial;
    serialType = SerialType::BTSerial;
}

void TelePlot::PrintLabel(String label)
{
    switch(serialType)
    {
        case SerialType::HWSerial:
            hwSerial.print(">" + label);
            break;
        case SerialType::BTSerial:
            btSerial.print(">" + label);
            break;
        default:
            Serial.print(">" + label);
    }
}

//TODO Create private method to print the value to the proper serial


void TelePlot::Plot(String name, int value)
{
    //TODO Cal private methods to print to the proper Serial
    this->PrintLabel(name);
}

void TelePlot::Plot(String name, double value)
{
    //TODO Cal private methods to print to the proper Serial
}
void TelePlot::Plot(String name, float value)
{
    //TODO Cal private methods to print to the proper Serial

}

void TelePlot::Plot(String name, long value)
{
    //TODO Cal private methods to print to the proper Serial

}

void TelePlot::Plot(String name, unsigned int value)
{
    //TODO Cal private methods to print to the proper Serial

}

void TelePlot::Plot(String name, unsigned long value)     
{
    //TODO Cal private methods to print to the proper Serial

}

