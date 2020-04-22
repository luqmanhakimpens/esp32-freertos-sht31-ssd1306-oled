/*
 * main.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Luqman_Hakim
 */

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "time.h"
#include "Adafruit_SHT31.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Fonts/FreeMonoBold9pt7b.h"
#include "Fonts/FreeSerif9pt7b.h"
#include "serial_debug.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include "tool/tool.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SHT31 sht31 = Adafruit_SHT31();
AsyncWebServer server(80);
tool tool;

TaskHandle_t task_handle_sensing;
TaskHandle_t task_handle_clock;
SemaphoreHandle_t xMutex = xSemaphoreCreateMutex();

typedef struct
{
	String ssid;
	String password;
} config_t;

config_t config;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600 * 7;
const int daylightOffset_sec = 0;

int split_line_x = display.width() * 2 / 4 + 5;

String spiffs_read(fs::FS &fs, const char * path)
{
	String ret_str;
	Serial.printf("Reading file: %s\r\n", path);

	File file = fs.open(path);
	if(!file || file.isDirectory())
	{
		Serial.println("- failed to open file for reading");
		return "";
	}

	while(file.available())
	{
		ret_str += (char) file.read();
	}
	file.close();

	Serial.println(ret_str);
	return ret_str;
}

void spiffs_write(fs::FS &fs, const char * path, const char * message)
{
	Serial.printf("Writing file: %s\r\n", path);

	File file = fs.open(path, FILE_WRITE);
	if(!file)
	{
		Serial.println("- failed to open file for writing");
		return;
	}
	if(file.print(message))
	{
		Serial.println("- file written");
	}
	else
	{
		Serial.println("- frite failed");
	}
}

void spiffs_delete(fs::FS &fs, const char * path)
{
	Serial.printf("Deleting file: %s\r\n", path);
	if(fs.remove(path))
	{
		Serial.println("- file deleted");
	}
	else
	{
		Serial.println("- delete failed");
	}
}

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

void config_init()
{
	if(!SPIFFS.begin(true))
	{
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}
	config_read(&config);
}

String processor(const String& var)
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
		return config.ssid;
	}
	else if(var == "PASSWRDDSPLY")
	{
		int len = config.password.length();
		String buf = "";
		for(int i = 1; i <= len; i++)
		{
			buf += "*";
		}
		return buf;
	}
	else if(var == "DEVICE_CODE")
	{
		return tool.device_id();
	}

	return String();
}

void async_webserver_init()
{

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
	{
		request->send(SPIFFS, "/index.html", String(), false, processor);
	});

	server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request)
	{
		//http://192.168.43.228/get?apn=axis&ssid=CUBE&ssid_password=123456789&backend_server=158.140.167.173&backend_port=1884&backend_username=eyroMQTT&backend_password=eyroMQTT1234
		// GET input1 value on <ESP_IP>/get?input1=<inputMessage>

			if (request->hasParam("ssid") && request->hasParam("ssid_password"))
			{
				config_t cfg;
				cfg.ssid = request->getParam("ssid")->value();
				cfg.password = request->getParam("ssid_password")->value();
				config_write(&cfg);
			}
			request->send(200, "text/html", "Setting parameter success, please return home page and reboot soon! <br><a href=\"/\">Return to Home Page</a>");
		});

	// Start server
	server.begin();
}

bool wifi_init()
{
	WiFi.softAP("esp32", NULL);
	Serial.println("SOFT AP UP");
	Serial.print("Connecting to ");
	Serial.println(config.ssid);
	WiFi.begin(config.ssid.c_str(), config.password.c_str());
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

bool sht_init()
{
	Serial.println("display init");
	if(!sht31.begin(0x44))
	{
		Serial.println("sht failed");
		return false;
	}
	return true;
}

bool display_init()
{
	Serial.println("display init");
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
	{
		Serial.println("display failed");
		return false;
	}

	display.cp437(true);
	display.setTextSize(1);
	display.setTextColor(WHITE, BLACK);
	display.clearDisplay(); //clear adafruit opening

	// Display opening bitmap
	display.drawBitmap(0, 0, trisna, 128, 64, WHITE);
	display.display();

	delay(2000);
	display.clearDisplay();
	display.display();
	return true;
}

void display_write(const char *text, uint8_t text_size = 1)
{
	if(text_size != 1) display.setTextSize(text_size);
	display.println(text);
	if(text_size != 1) display.setTextSize(1);
}

void display_write(int pos_x, int pos_y, const char *text, uint8_t text_size = 1)
{
	display.setCursor(pos_x, pos_y);
	display_write(text, text_size);
}

void display_erase(int x, int y, int w, int h)
{
	display.fillRect(x, y, w, h, BLACK);
}

void display_time()
{
	char buff[256];
	struct tm timeinfo;
	int x_offset = split_line_x + 10;

	if(!getLocalTime(&timeinfo))
	{
		Serial.println("Failed to obtain time");
		return;
	}

	display_erase(split_line_x + 1, 0, display.width(), display.height());

	display.setFont(&FreeSerif9pt7b);

	strftime(buff, sizeof(buff), "%H:%M", &timeinfo);
	display_write(x_offset, 15, buff);

	display.setFont(&FreeSerif9pt7b);
	strftime(buff, sizeof(buff), "%a,", &timeinfo);
	display_write(x_offset, 35, buff);

	strftime(buff, sizeof(buff), "%d/%m", &timeinfo);
	display_write(x_offset, 55, buff);

	display.setFont();
	display.display();

}

void display_sensing()
{
	char buff[256];

	float t = sht31.readTemperature();
	float h = sht31.readHumidity();
	t = round(t * 100) / 100;
	h = round(h * 100) / 100;

	if(!isnan(t))
	{  // check if 'is not a number'
		Serial.print("Temp *C = ");
		Serial.println(t);
	}
	else
	{
		Serial.println("Failed to read temperature");
		return;
	}

	if(!isnan(h))
	{  // check if 'is not a number'
		Serial.print("Hum. % = ");
		Serial.println(h);
	}
	else
	{
		Serial.println("Failed to read humidity");
		return;
	}

	display_erase(0, 0, split_line_x, display.height());

	display_write(0, 0, "temperature");
	sprintf(buff, "%.1fC", t);
	display_write(0, 10, buff, 2);

	display_write(0, 35, "humidity");
	sprintf(buff, "%.1f%%", h);
	display_write(0, 45, buff, 2);

	display.drawFastVLine(split_line_x, 0, display.height(), WHITE);
	display.display();
}

void task_sensing(void *parameter)
{
	while(1)
	{
		xSemaphoreTake(xMutex, portMAX_DELAY);
		display_sensing();
		xSemaphoreGive(xMutex);
		vTaskDelay(2000 / portTICK_RATE_MS);
	}
}

void task_clock(void *paramater)
{
	while(1)
	{
		xSemaphoreTake(xMutex, portMAX_DELAY);
		display_time();
		xSemaphoreGive(xMutex);
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

void task_create_sensing()
{
	xTaskCreatePinnedToCore(task_sensing, /* Task function. */
							"task_sensing", /* name of task. */
							5000, /* Stack size of task */
							NULL, /* parameter of the task */
							2, /* priority of the task */
							&task_handle_sensing, /* Task handle to keep track of created task */
							1); /* pin task to core 1 */
	delay(100);
}

void task_create_clock()
{
	xTaskCreatePinnedToCore(task_clock, /* Task function. */
							"task_clock", /* name of task. */
							5000, /* Stack size of task */
							NULL, /* parameter of the task */
							1, /* priority of the task */
							&task_handle_clock, /* Task handle to keep track of created task */
							1); /* pin task to core 1 */
	delay(100);
}

void setup()
{
	Serial.begin(115200);

	config_init();

	if(display_init())
	{
		if(sht_init())
		{
			DEBUG_PRINTLN("sht init");
			display_write(0, 0, "sht init ok");
			task_create_sensing();
		}
		else
		{
			display_write(0, 0, "sht init ko");
		}
		display.display();

		if(wifi_init())
		{
			DEBUG_PRINTLN("wifi init");
			display_write(0, 10, "wifi init ok");
			configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
			display_write(0, 20, "time init ok");
			task_create_clock();
		}
		else
		{
			display_write(0, 10, "wifi init k0");
		}
		display.display();
	}

	async_webserver_init();
}

void loop()
{

}
