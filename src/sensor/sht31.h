/*
 * sht31.h
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#ifndef SRC_SENSOR_SHT31_H_
#define SRC_SENSOR_SHT31_H_

#include "Arduino.h"
#include "Adafruit_SHT31.h"

typedef struct
{
	float temperature;
	float humidity;
}sht_data_t;

bool sht_init();
bool sht_read(sht_data_t *sht);

#endif /* SRC_SENSOR_SHT31_H_ */
