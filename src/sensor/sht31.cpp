/*
 * sht31.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#include "sht31.h"
Adafruit_SHT31 sht31 = Adafruit_SHT31();

bool sht_init()
{
	Serial.println("display init");
	if(!sht31.begin(0x44))
	{
		Serial.println("sht failed");
		return false;
	}
	return true;
}

bool sht_read(sht_data_t *sht)
{
	sht->temperature = sht31.readTemperature();
	sht->humidity = sht31.readHumidity();
	sht->temperature = round(sht->temperature * 100) / 100;
	sht->humidity = round(sht->humidity * 100) / 100;

	if(!isnan(sht->temperature) && !isnan(sht->humidity))
	{  // check if 'is not a number'
		Serial.print("Temp *C = ");
		Serial.println(sht->temperature);
		Serial.print("Hum. % = ");
		Serial.println(sht->humidity);
		return true;
	}
	else
	{
		Serial.println("Failed to read sensor value");
		return false;
	}
}
