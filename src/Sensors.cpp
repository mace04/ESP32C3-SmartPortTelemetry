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
	isSensorFuel = isSensorCurr && settings.EnableSensorFuel;
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
		case SENSOR_ALT: //TODO Read ALT sensor
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
	unsigned long durationTime = micros();

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
	// Calculate tolerancde of +-2 points. if within tolerance use previous reading
		if(val/SAMPLE_RATES > lastReadings.curr + (2) || val/SAMPLE_RATES < lastReadings.curr - (2))
		{
			lastReadings.curr = val / SAMPLE_RATES;
		}
		else
		{
			val = lastReadings.curr * SAMPLE_RATES;
		}
		// TelePlot::Plot("PIN_CURR(A0):", (int) (val/SAMPLE_RATES));

        // Read the raw ADC sample values from the ACS758_PIN
        int raw_adc = val / SAMPLE_RATES;
        // Convert the raw ADC value to voltage in millivolts
        float voltage = raw_adc * (Settings::GetSensorSettings().CurrVoltageRef / 4095.0);
        // Calculate the current based on the voltage and the sensor's sensitivity and zero-current voltage output
        retval = (voltage - Settings::GetSensorSettings().CurrOffset)/ Settings::GetSensorSettings().CurrSensitivity;  

		// retval = ((float) val / (float) SAMPLE_RATES) * Settings::GetSensorSettings().AmpsPerPoint / 10000.00;
		// TelePlot::Plot("CURR:", retval*10);
	}
	else {
		retval = -1;
	}
	// TelePlot::Plot("CURR Sensor Duration:", micros() - durationTime);
	this->SetPowerConsumption(retval);
	// TelePlot::Plot("CURR+FUEL Sensor Duration:", micros() - durationTime);
	return retval;
}

float Sensors::GetVoltage(int pin) { 
	int i;
	unsigned long val = 0;
	float vpp;
	unsigned long durationTime = micros();

	for (i = 0; i < SAMPLE_RATES; i++) {
		val += analogRead(pin);
		delayMicroseconds(SAMPLE_DELAY_MS);
	}

#ifdef _TEST_VALUES_
	if (voltageTestValue == 1023)
		voltageTestValue = 0;
	val = SAMPLE_RATES * voltageTestValue++;
#endif

	String sensorName, pinName;
	unsigned int previousValue;
	switch (pin) {
	case PIN_VFAS:
		vpp = Settings::GetSensorSettings().VoltsPerPoint;
		sensorName = "VFAS:";
		pinName = "PIN_VFAS(A1):";
		previousValue = lastReadings.vfas;
		break;
	case PIN_A3:
		vpp = Settings::GetSensorSettings().VoltsPerPoint;
		sensorName = "A3:";
		pinName = "PIN_A3(A2):";
		previousValue = lastReadings.a3;
		break;
#if !defined(ARDUINO_XIAO_ESP32C3) && !defined(ESP32)
	case PIN_A4:
		vpp = Settings::GetSensorSettings().VoltsPerPoint;
		sensorName = "A4:";
		pinName = "PIN_A4(A3):";
		previousValue = lastReadings.a4;
		break;
#endif
	default:
		vpp = Settings::GetSensorSettings().VoltsPerPoint;
		sensorName = "VFAS:";
		pinName = "PIN_VFAS(A1):";
		previousValue = lastReadings.vfas;
		break;
	}
	// Calculate tolerancde of +-2 points. if within tolerance use previous reading
	if(val/SAMPLE_RATES > previousValue + (2) || val/SAMPLE_RATES < previousValue - (2))
	{
		lastReadings.vfas = val / SAMPLE_RATES;
	}
	else
	{
		val = lastReadings.vfas * SAMPLE_RATES;
	}
	// TelePlot::Plot(pinName, (int)(val/SAMPLE_RATES));

	float retval = ((float)val / (float) SAMPLE_RATES) * vpp / 1000.00  ;
	// TelePlot::Plot(sensorName, retval);
	// TelePlot::Plot(sensorName + " Sensor Duration:", micros() - durationTime);
	return retval;
}

void Sensors::SetPowerConsumption(float curr) 
{
    float time = (float)(millis() - timer) / 1000.0;
    // consumption += current * 1000.00 * (float)time / 3600.00;
    consumption += (float) curr * CALIBRATION_FUEL / (time * 1000.00 / 60.00);
	// TelePlot::Plot("Fuel:", consumption);
    timer = millis();
}

float Sensors::GetPowerConsumption()
{
	return consumption;
}

