/*
 * config.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#include "config.h"

void config_read(config_t * cfg)
{
	String config_str = spiffs_read(SPIFFS, "/config.json");

	const size_t capacity = JSON_OBJECT_SIZE(4) + 100;
	DynamicJsonDocument j_obj(capacity);

	DeserializationError err;
	err = deserializeJson(j_obj, config_str);
	if(err)
	{
		Serial.println(err.c_str());
	}

	cfg->ssid = (const char*) j_obj["ssid"];
	cfg->password = (const char*) j_obj["password"];

	Serial.println(cfg->ssid);
	Serial.println(cfg->password);
}

void config_write(config_t *config)
{
	spiffs_delete(SPIFFS, "/config.json");

	const size_t capacity = JSON_OBJECT_SIZE(4) + 25;
	DynamicJsonDocument j_obj(capacity);
	String json_str;

	j_obj["ssid"] = config->ssid;
	j_obj["password"] = config->password;

	serializeJson(j_obj, json_str);

	spiffs_write(SPIFFS, "/config.json", json_str.c_str());
}

void config_init(config_t * config)
{
	if(!SPIFFS.begin(true))
	{
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}
	config_read(config);
}
