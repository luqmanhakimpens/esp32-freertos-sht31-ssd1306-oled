/*
 * network.h
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#ifndef SRC_NETWORK_NETWORK_H_
#define SRC_NETWORK_NETWORK_H_

#include <WiFi.h>
#include "config/config.h"

bool wifi_init(config_t * config);

#endif /* SRC_NETWORK_NETWORK_H_ */
