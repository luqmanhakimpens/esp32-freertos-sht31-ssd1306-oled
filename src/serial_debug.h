/*
 * serial_log.h
 *
 *  Created on: Jul 23, 2019
 *      Author: luqma
 */

#ifndef SERIAL_DEBUG_H_
#define SERIAL_DEBUG_H_

#include "app_config.h"

#ifdef DEBUG_ENABLE
	#define DEBUG_PRINTLN(str)	DEBUG_PORT.println(str)
#else
	#define DEBUG_PRINTLN(str)	{}
#endif

#ifdef DEBUG_ENABLE
	#define DEBUG_PRINT(str)	DEBUG_PORT.print(str)
#else
	#define DEBUG_PRINT(str)	{}
#endif

#endif /* LIB_SERIAL_LOG_SERIAL_LOG_H_ */
