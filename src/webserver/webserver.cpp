/*
 * webserver.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#include "webserver.h"

config_t * _config;
AsyncWebServer server(80);

static String html_template_processor(const String& var)
{
	if(var == "APN")
	{
		return "apn";
	}
	else if(var == "MODEL")
	{
		return "model";
	}
	else if(var == "FW_VERSION")
	{
		return FW_VERSION;
	}
	else if(var == "SSID")
	{
		return _config->ssid;
	}
	else if(var == "PASSWRDDSPLY")
	{
		int len = _config->password.length();
		String buf = "";
		for(int i = 1; i <= len; i++)
		{
			buf += "*";
		}
		return buf;
	}
	else if(var == "DEVICE_CODE")
	{
		tool tool;
		return tool.device_id();
	}

	return String();
}

void async_webserver_init(config_t * config)
{
	_config = config;

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
	{
		request->send(SPIFFS, "/index.html", String(), false, html_template_processor);
	});

	server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request)
	{
		if (request->hasParam("ssid") && request->hasParam("ssid_password"))
			{
				config_t cfg;
				cfg.ssid = request->getParam("ssid")->value();
				cfg.password = request->getParam("ssid_password")->value();
				config_write(&cfg);
			}
			request->send(200, "text/html", "Setting parameter success, please return home page and reboot soon! <br><a href=\"/\">Return to Home Page</a>");
		});

	server.begin();
}
