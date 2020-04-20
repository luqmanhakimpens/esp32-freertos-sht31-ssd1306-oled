/*
 * config.h
 *
 *  Created on: Jul 23, 2019
 *      Author: luqma
 */

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#include "esp_build_defs.h"

const char build_date[] =
{
    BUILD_YEAR_CH2,
	BUILD_YEAR_CH3,

    BUILD_MONTH_CH0,
	BUILD_MONTH_CH1,

    BUILD_DAY_CH0,
	BUILD_DAY_CH1,
    '-',
    BUILD_HOUR_CH0,
	BUILD_HOUR_CH1,

    BUILD_MIN_CH0,
	BUILD_MIN_CH1,
	'\0'
};

#define DEBUG_ENABLE
// define serial log level here, or you can define it in platformio build flag.
//level 3 to 1; 1 is highest, 0 means no log output
//#define SERIAL_LOG_LEVEL LOG_LEVEL_HIGH


#endif /* APP_CONFIG_H_ */
