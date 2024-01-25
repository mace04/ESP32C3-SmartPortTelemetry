#include <SmartPort.h>

// SmartPort::SmartPort()
// {
//     loop = 0;
//     this->begin();
// }

SmartPort::SmartPort()
{

}

void SmartPort::Begin(uint8_t rxPin, uint8_t txPin, bool inverted){
  this->settings = Settings::GetSmartPortSettings();
#ifdef ARDUINO_XIAO_ESP32C3
  smartPort->begin(settings.BaudRate, SERIAL_8N1, rxPin, txPin, inverted);
#else
  smartPort->begin(settings.BaudRate);
#endif
  sensors->Begin();
  this->RegisterSensors();
}

void SmartPort::RegisterSensors()
{
  sensors->RegisterSensors();
  if(sensors->IsRegistered(SENSOR_ALT)) RegisterSensor(FRSKY_VALUE_TYPE_ALT);
  if(sensors->IsRegistered(SENSOR_CURR)) RegisterSensor(FRSKY_VALUE_TYPE_CURR);
  if(sensors->IsRegistered(SENSOR_VFAS)) RegisterSensor(FRSKY_VALUE_TYPE_VFAS);
  if(sensors->IsRegistered(SENSOR_FUEL)) RegisterSensor(FRSKY_VALUE_TYPE_FUEL);
  if(sensors->IsRegistered(SENSOR_GPS_LONG_LATI)) RegisterSensor(FRSKY_VALUE_TYPE_GPS_LONG_LATI);
  if(sensors->IsRegistered(SENSOR_GPS_ALT)) RegisterSensor(FRSKY_VALUE_TYPE_GPS_ALT);
  if(sensors->IsRegistered(SENSOR_GPS_SPEED)) RegisterSensor(FRSKY_VALUE_TYPE_GPS_SPEED);
  if(sensors->IsRegistered(SENSOR_A3)) RegisterSensor(FRSKY_VALUE_TYPE_A3);
  if(sensors->IsRegistered(SENSOR_A4)) RegisterSensor(FRSKY_VALUE_TYPE_A4);
  if(sensors->IsRegistered(SENSOR_AIR_SPEED)) RegisterSensor(FRSKY_VALUE_TYPE_AIR_SPEED);
}

void SmartPort::RegisterSensor(uint16_t sensorId)
{
  switch(sensorId)
  {
    case FRSKY_VALUE_TYPE_ALT:
      altData.isRegistered = sensors->IsRegistered(SENSOR_ALT);
      altData.sensorId = FRSKY_VALUE_TYPE_ALT;
      if(altData.isRegistered) Serial.println("Sensor Registered: ALT");
      break;
    case FRSKY_VALUE_TYPE_CURR:
      currData.isRegistered = sensors->IsRegistered(SENSOR_CURR);
      currData.sensorId = FRSKY_VALUE_TYPE_CURR;
      if(currData.isRegistered) Serial.println("Sensor Registered: CURR");
      break;
    case FRSKY_VALUE_TYPE_VFAS:
      vfasData.isRegistered = sensors->IsRegistered(SENSOR_VFAS);
      vfasData.sensorId = FRSKY_VALUE_TYPE_VFAS;
      if(vfasData.isRegistered) Serial.println("Sensor Registered: VFAS");
      break;
    case FRSKY_VALUE_TYPE_FUEL:
      fuelData.isRegistered = sensors->IsRegistered(SENSOR_FUEL);
      fuelData.sensorId = FRSKY_VALUE_TYPE_FUEL;
      if(fuelData.isRegistered) Serial.println("Sensor Registered: FUEL");
      break;
    case FRSKY_VALUE_TYPE_GPS_LONG_LATI:
      gpsPosData.isRegistered = sensors->IsRegistered(SENSOR_GPS_LONG_LATI);
      gpsPosData.sensorId = FRSKY_VALUE_TYPE_GPS_LONG_LATI;
      if(gpsPosData.isRegistered) Serial.println("Sensor Registered: GPS_LONG_LATI");
      break;
    case FRSKY_VALUE_TYPE_GPS_ALT:
      gpsAltData.isRegistered = sensors->IsRegistered(SENSOR_GPS_ALT);
      gpsAltData.sensorId = FRSKY_VALUE_TYPE_GPS_ALT;
      if(gpsAltData.isRegistered) Serial.println("Sensor Registered: GPS_ALT");
      break;
    case FRSKY_VALUE_TYPE_GPS_SPEED:
      gpsSpeedData.isRegistered = sensors->IsRegistered(SENSOR_GPS_SPEED);
      gpsSpeedData.sensorId = FRSKY_VALUE_TYPE_GPS_SPEED;
      if(gpsSpeedData.isRegistered) Serial.println("Sensor Registered: GPS_SPEED");
      break;
    case FRSKY_VALUE_TYPE_A3:
      a3Data.isRegistered = sensors->IsRegistered(SENSOR_A3);
      a3Data.sensorId = FRSKY_VALUE_TYPE_A3;
      if(a3Data.isRegistered) Serial.println("Sensor Registered: A3");
      break;
    case FRSKY_VALUE_TYPE_A4:
      a4Data.isRegistered = sensors->IsRegistered(SENSOR_A4);
      a4Data.sensorId = FRSKY_VALUE_TYPE_A4;
      if(a4Data.isRegistered) Serial.println("Sensor Registered: A4");
      break;
    case FRSKY_VALUE_TYPE_AIR_SPEED:
      airSpeedData.isRegistered = sensors->IsRegistered(SENSOR_AIR_SPEED);
      airSpeedData.sensorId = FRSKY_VALUE_TYPE_AIR_SPEED;
      if(airSpeedData.isRegistered) Serial.println("Sensor Registered: AIR_SPEED");
      break;
    default:
      break;
  }
}

bool SmartPort::IsRegistered(uint16_t sensorId)
{
  bool retVal = false;
  switch(sensorId)
  {
    case FRSKY_VALUE_TYPE_ALT:
      retVal = altData.isRegistered;
      break;
    case FRSKY_VALUE_TYPE_CURR:
      retVal = currData.isRegistered;
      break;
    case FRSKY_VALUE_TYPE_VFAS:
      retVal = vfasData.isRegistered;
      break;
    case FRSKY_VALUE_TYPE_FUEL:
      retVal = fuelData.isRegistered;
      break;
    case FRSKY_VALUE_TYPE_GPS_LONG_LATI:
      retVal = gpsPosData.isRegistered;
      break;
    case FRSKY_VALUE_TYPE_GPS_ALT:
      retVal = gpsAltData.isRegistered;
      break;
    case FRSKY_VALUE_TYPE_GPS_SPEED:
      retVal = gpsSpeedData.isRegistered;
      break;
    case FRSKY_VALUE_TYPE_A3:
      retVal = a3Data.isRegistered;
      break;
    case FRSKY_VALUE_TYPE_A4:
      retVal = a4Data.isRegistered;
      break;
    case FRSKY_VALUE_TYPE_AIR_SPEED:
      retVal = airSpeedData.isRegistered;
      break;
    default:
      break;
  }
  return retVal;
}

void SmartPort::ReadSensors()
{
  //TODO Read ALT sensor
  if (IsRegistered(FRSKY_VALUE_TYPE_CURR))
    SetSensorValue(FRSKY_VALUE_TYPE_CURR, sensors->ReadSensor(SENSOR_CURR) * 100);
  if (IsRegistered(FRSKY_VALUE_TYPE_VFAS))
    SetSensorValue(FRSKY_VALUE_TYPE_VFAS, sensors->ReadSensor(SENSOR_VFAS) * 100);
  if (IsRegistered(FRSKY_VALUE_TYPE_ALT))
    SetSensorValue(FRSKY_VALUE_TYPE_ALT, sensors->ReadSensor(SENSOR_ALT));
  if (IsRegistered(FRSKY_VALUE_TYPE_A3))
    SetSensorValue(FRSKY_VALUE_TYPE_A3, sensors->ReadSensor(SENSOR_A3) * 100);
  if (IsRegistered(FRSKY_VALUE_TYPE_A4))
    SetSensorValue(FRSKY_VALUE_TYPE_A4, sensors->ReadSensor(SENSOR_A4) * 100);
  if (IsRegistered(FRSKY_VALUE_TYPE_FUEL))
    SetSensorValue(FRSKY_VALUE_TYPE_FUEL, sensors->ReadSensor(SENSOR_FUEL));
}

void SmartPort::SetSensorValue(uint16_t sensorId, uint32_t value)
{
  switch(sensorId)
  {
    case FRSKY_VALUE_TYPE_ALT:
      altData.value.longValue = value;
      altData.crc = this->GetChecksum(altData);
      break;
    case FRSKY_VALUE_TYPE_CURR:
      currData.value.longValue = value;
      currData.crc = this->GetChecksum(currData);
      break;
    case FRSKY_VALUE_TYPE_VFAS:
      vfasData.value.longValue = value;
      vfasData.crc = this->GetChecksum(vfasData);
      break;
    case FRSKY_VALUE_TYPE_FUEL:
      fuelData.value.longValue = value;
      fuelData.crc = this->GetChecksum(fuelData);
      break;
    case FRSKY_VALUE_TYPE_GPS_LONG_LATI:
      gpsPosData.value.longValue = value;
      gpsPosData.crc = this->GetChecksum(gpsPosData);
      break;
    case FRSKY_VALUE_TYPE_GPS_ALT:
      gpsAltData.value.longValue = value;
      gpsAltData.crc = this->GetChecksum(gpsAltData);
      break;
    case FRSKY_VALUE_TYPE_GPS_SPEED:
      gpsSpeedData.value.longValue = value;
      gpsSpeedData.crc = this->GetChecksum(gpsSpeedData);
      break;
    case FRSKY_VALUE_TYPE_A3:
      a3Data.value.longValue = value;
      a3Data.crc = this->GetChecksum(a3Data);
      break;
    case FRSKY_VALUE_TYPE_A4:
      a4Data.value.longValue = value;
      a4Data.crc = this->GetChecksum(a4Data);
      break;
    case FRSKY_VALUE_TYPE_AIR_SPEED:
      airSpeedData.value.longValue = value;
      airSpeedData.crc = this->GetChecksum(airSpeedData);
      break;
    default:
      break;
  }
}

void SmartPort::Hanlde() //TODO Serial feedback for SmartPort telemetry values
{
  // ReadSensors();
  if (smartPort->available() > 0)
  {
    if (smartPort->read() == 0x7E){
      while(smartPort->available() == 0);
      int polledSensor = smartPort->read();
// Serial.printf("Polling 0x7e 0x%02x\n", polledSensor);
      switch(polledSensor)
      {
        case 0x22:  // Physical ID 3 - FAS-40S current sensor
            sensorPolling0x22Loop++;
            if (sensorPolling0x22Loop % 2 == 0 && millis() - currData.lastSent > settings.RefreshRate && currData.isRegistered)
            {
              SetSensorValue(FRSKY_VALUE_TYPE_CURR, sensors->ReadSensor(SENSOR_CURR) * 100);
              this->SendData(currData);
            }
            if (sensorPolling0x22Loop % 2 == 1 && millis() - vfasData.lastSent > settings.RefreshRate && vfasData.isRegistered) 
            {
              SetSensorValue(FRSKY_VALUE_TYPE_VFAS, sensors->ReadSensor(SENSOR_VFAS) * 100);
              this->SendData(vfasData);
            }
// Serial.printf("%03i    VFAS Registered: %i, header: %02x, sensorId: %04x value: %04i, crc: %02x\n", 
//   sensorPolling0x22Loop, vfasData.isRegistered, vfasData.frameHeader, vfasData.sensorId, vfasData.value.longValue, vfasData.crc);
          break;
        case 0x83:  // Physical ID 4 - GPS / altimeter (normal precision)
          // if (loop % 3 == 0 && gpsAltData.isRegistered) this->SendData(gpsAltData);
          // if (loop % 3 == 1 && gpsSpeedData.isRegistered) this->SendData(gpsSpeedData);
          // TODO Send ALT data to Smartport receiver
          // if (loop % 3 == 2 && altData.isRegistered) this->SendData(altData);
// Serial.printf("%09u SensorId: 0x83   GPSALT: %05i   GPSSPD: %05i   ALT: %05i\n", millis(), gpsAltData.value.longValue, gpsSpeedData.value.longValue, altData.value.longValue);
          break;
        case 0x45:  // Physical ID 6 - SP2UART(Host)
          if (millis() - a3Data.lastSent > settings.RefreshRate && a3Data.isRegistered) 
          {
            SetSensorValue(FRSKY_VALUE_TYPE_A3, sensors->ReadSensor(SENSOR_A3));
            this->SendData(a3Data);
          }
          if (millis() - a4Data.lastSent > settings.RefreshRate && a4Data.isRegistered) 
          {
            SetSensorValue(FRSKY_VALUE_TYPE_A4, sensors->ReadSensor(SENSOR_A4));
            this->SendData(a4Data);
          }
          break;
        case 0x98:  // Physical ID 25 - Fuel / Power consumption
          if (millis() - fuelData.lastSent > settings.RefreshRate && fuelData.isRegistered) 
          {
            SetSensorValue(FRSKY_VALUE_TYPE_FUEL, sensors->ReadSensor(SENSOR_FUEL));
            this->SendData(fuelData);
          }
// Serial.printf("%09u SensorId: 0x98   FUEL: %05i\n", millis(), fuelData.value.longValue);
          break;
        default:
          break;
      }
    }
  }
}


byte SmartPort::GetChecksum(SmartPortFrame data)
{
  byte frame[7];
  frame[0] = data.frameHeader;    
  frame[1] = lowByte(data.sensorId);
  frame[2] = highByte(data.sensorId);
  frame[3] = data.value.byteValue[0];
  frame[4] = data.value.byteValue[1];
  frame[5] = data.value.byteValue[2];
  frame[6] = data.value.byteValue[3];  

  long total = 0;
  for(int i = 0; i < 7; i++)
    total += frame[i];

  if(total >= 0x700) {
    total+= 7;
  } else if(total >= 0x600) {
    total+= 6;
  } else if(total >= 0x500) {
    total+= 5;
  } else if(total >= 0x400) {
    total+= 4;
  } else if(total >= 0x300) {
    total+= 3;
  } else if(total >= 0x200) {
    total+= 2;
  } else if(total >= 0x100) {
    total++;
  }
  return 0xFF - total;    
}

void SmartPort::SendData(SmartPortFrame& data)
{
  if(softwarePin > 0) {
      // smartPort->stopListening();
      pinMode(softwarePin, OUTPUT);
      delay(1);
  }  
  byte frame[8];
  frame[0] = data.frameHeader;    
  frame[1] = lowByte(data.sensorId);
  frame[2] = highByte(data.sensorId);
  frame[3] = data.value.byteValue[0];
  frame[4] = data.value.byteValue[1];
  frame[5] = data.value.byteValue[2];
  frame[6] = data.value.byteValue[3];  
  frame[7] = data.crc;

  long total = 0;
  for(int i = 0; i < 8; i++)
    this->SendByte(frame[i]);
  data.lastSent = millis();
}

//Send a data byte the FrSky way
void SmartPort::SendByte(byte b) 
{
  if(b == 0x7E) {
    smartPort->write(0x7D);
    smartPort->write(0x5E);
  } else if(b == 0x7D) {
    smartPort->write(0x7D);
    smartPort->write(0x5D);
  } else {
    smartPort->write(b);
  }
}
