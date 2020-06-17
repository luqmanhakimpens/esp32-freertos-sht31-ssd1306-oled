/*
 * time.h
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#ifndef SRC_TIME_MANAGER_TIME_MANAGER_H_
#define SRC_TIME_MANAGER_TIME_MANAGER_H_

#include "Arduino.h"
#include "time.h"

typedef struct tm timeinfo_t;

void time_init(const char* ntp_server, long gmt_offset_sec, int daylight_offset_sec);
void time_get(timeinfo_t * timeinfo);


#endif /* SRC_TIME_MANAGER_TIME_MANAGER_H_ */
