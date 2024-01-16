#include <Sensors.h>

Sensors::Sensors()
{
	consumption = 0;
}

void Sensors::Begin()
{
	settings = Settings::GetSensorSettings();
    timer = millis();
}

void Sensors::RegisterSensors()
{
	isSensorVfas = settings.EnableSensorVFAS; //(PIN_VFAS) > 0;
	isSensorCurr = settings.EnableSensorCURR; //analogRead(PIN_CURR) > 0;
	isSensorFuel = isSensorCurr;
	isSensorA3 = settings.EnableSensorA3; //analogRead(PIN_A3) > 0;
	#ifndef ARDUINO_XIAO_ESP32C3
		isSensorA4 = settings.EnableSensorA4; //analogRead(PIN_A4) > 0;
	#endif
	isSensorAlt = bmp180.begin();

	if (isSensorAlt) {
		baselinePressure = this->GetPressure();
	}
}

bool Sensors::IsRegistered(uint16_t sensorId)
{
	switch(sensorId)
	{
		case SENSOR_VFAS:
			return isSensorVfas;
		case SENSOR_CURR:
			return isSensorCurr;
		case SENSOR_FUEL:
			return isSensorFuel;
		case SENSOR_A3:
			return isSensorA3;
		case SENSOR_A4:
			return isSensorA4;
		case SENSOR_ALT:
			return isSensorAlt;
		case SENSOR_GPS_LONG_LATI:
			return isSensorGpsPos;
		case SENSOR_GPS_ALT:
			return isSensorGpsAlt;
		case SENSOR_GPS_SPEED:
			return isSensorGpsSpd;
		case SENSOR_AIR_SPEED:
			return isSensorSpd;
		default:
			return false;
	}
}

float Sensors::ReadSensor(uint16_t sensorId)
{
	switch(sensorId)
	{
		case SENSOR_VFAS:
			return this->GetVoltage(PIN_VFAS);
		case SENSOR_CURR:
			current = this->GetCurrent();
			return current;
		case SENSOR_FUEL:
			return this->GetPowerConsumption();
		case SENSOR_A3:
			return this->GetVoltage(PIN_A3);
#ifndef ARDUINO_XIAO_ESP32C3
		case SENSOR_A4:
			return GetVoltage(PIN_A4);
#endif			
		case SENSOR_ALT:
			return -1;
		case SENSOR_GPS_LONG_LATI:
			return -1;
		case SENSOR_GPS_ALT:
			return -1;
		case SENSOR_GPS_SPEED:
			return -1;
		case SENSOR_AIR_SPEED:
			return -1;
		default:
			return false;
	}	
}

double Sensors::GetPressure()
{
	char status;
	double T, P, p0, a;

	// You must first get a temperature measurement to perform a pressure reading.

	// Start a temperature measurement:
	// If request is successful, the number of ms to wait is returned.
	// If request is unsuccessful, 0 is returned.

	status = bmp180.startTemperature();
	if (status != 0)
	{
		// Wait for the measurement to complete:

		delay(status);

		// Retrieve the completed temperature measurement:
		// Note that the measurement is stored in the variable T.
		// Use '&T' to provide the address of T to the function.
		// Function returns 1 if successful, 0 if failure.

		status = bmp180.getTemperature(T);
		if (status != 0)
		{
			// Start a pressure measurement:
			// The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
			// If request is successful, the number of ms to wait is returned.
			// If request is unsuccessful, 0 is returned.

			status = bmp180.startPressure(3);
			if (status != 0)
			{
				// Wait for the measurement to complete:
				delay(status);

				// Retrieve the completed pressure measurement:
				// Note that the measurement is stored in the variable P.
				// Use '&P' to provide the address of P.
				// Note also that the function requires the previous temperature measurement (T).
				// (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
				// Function returns 1 if successful, 0 if failure.

				status = bmp180.getPressure(P, T);
				if (status != 0)
				{
					return(P);
				}
				else Serial.println("error retrieving pressure measurement\n");
			}
			else Serial.println("error starting pressure measurement\n");
		}
		else Serial.println("error retrieving temperature measurement\n");
	}
	else Serial.println("error starting temperature measurement\n");
	return -1;
}

int curretTestValue = 0;
float previousCurrentValue = 0;
float Sensors::GetCurrent() {
	int i;
	unsigned long val = 0;
	float retval;

	if (isSensorCurr) {
		for (i = 0; i < SAMPLE_RATES; i++) {
			val += analogRead(PIN_CURR);// *MAX_CURRENT * 10.00 / 750.00;
			delayMicroseconds(SAMPLE_DELAY_MS);
		}
#ifdef _TEST_VALUES_
		if (curretTestValue == 1023)
			curretTestValue = 0;
		val = SAMPLE_RATES * curretTestValue++;
#endif // _TEST_VALUES_
		retval = ((float) val / (float) SAMPLE_RATES) * Settings::GetSensorSettings().AmpsPerPoint / 1000.00;
	}
	else {
		retval = -1;
	}
	this->SetPowerConsumption();
	return retval;
}

float Sensors::GetVoltage(int pin) {
	int i;
	unsigned long val = 0;
	float vpp;

	for (i = 0; i < SAMPLE_RATES; i++) {
		val += analogRead(pin);
		delayMicroseconds(SAMPLE_DELAY_MS);
	}

#ifdef _TEST_VALUES_
	if (voltageTestValue == 1023)
		voltageTestValue = 0;
	val = SAMPLE_RATES * voltageTestValue++;
#endif

	switch (pin) {
	case PIN_VFAS:
		vpp = Settings::GetSensorSettings().VoltsPerPoint;
		// Debug("PIN " + String(PIN_VFAS) + ": " + String(val / SAMPLE_RATES));
		break;
	case PIN_A3:
		vpp = Settings::GetSensorSettings().VoltsPerPoint;
		// Debug("PIN " + String(PIN_A3) + ": " + String(val / SAMPLE_RATES));
		break;
	#ifndef ARDUINO_XIAO_ESP32C3
	case PIN_A4:
		vpp = Settings::GetSensorSettings().VoltsPerPoint;
		// Debug("PIN " + String(PIN_A4) + ": " + String(val / SAMPLE_RATES));
		break;
	#endif
	default:
		vpp = Settings::GetSensorSettings().VoltsPerPoint;
		break;
	}
	float retval = ((float)val / (float) SAMPLE_RATES) * vpp / (1000.00 * 10.00) ;
	return retval;
}

void Sensors::SetPowerConsumption()
{
    float time = (float)(millis() - timer) / 1000.0;
    // consumption += current * 1000.00 * (float)time / 3600.00;
    consumption += (float) current * CALIBRATION_FUEL / (time * 1000.00 / 60.00);
    timer = millis();
}

float Sensors::GetPowerConsumption()
{
	return consumption;
}

