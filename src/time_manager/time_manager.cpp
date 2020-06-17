/*
 * time.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */


#include "time_manager.h"

void time_init(const char* ntp_server, long gmt_offset_sec, int daylight_offset_sec)
{
	configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
}

void time_get(timeinfo_t * timeinfo)
{
	if(!getLocalTime(timeinfo))
	{
		Serial.println("Failed to obtain time");
		return;
	}
}

