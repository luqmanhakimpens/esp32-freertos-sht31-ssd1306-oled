/*
 * network.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#ifndef SRC_NETWORK_NETWORK_CPP_
#define SRC_NETWORK_NETWORK_CPP_

#include "network.h"

bool wifi_init(config_t * config)
{
	WiFi.softAP("esp32", NULL);
	Serial.println("SOFT AP UP");
	Serial.print("Connecting to ");
	Serial.println(config->ssid);
	WiFi.begin(config->ssid.c_str(), config->password.c_str());

	if(WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		Serial.printf("WiFi Failed!\n");
		return false;
	}
	Serial.println("");
	Serial.println("WiFi connected.");
	Serial.println(WiFi.localIP());

	return true;
}


#endif /* SRC_NETWORK_NETWORK_CPP_ */
