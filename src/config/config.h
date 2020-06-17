/*
 * config.h
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#ifndef SRC_CONFIG_CONFIG_H_
#define SRC_CONFIG_CONFIG_H_

#include "ArduinoJson.h"
#include "spiffs_io/spiffs_io.h"


typedef struct
{
	String ssid;
	String password;
} config_t;

void config_read(config_t * cfg);
void config_write(config_t *config);
void config_init(config_t * config);

#endif /* SRC_CONFIG_CONFIG_H_ */
