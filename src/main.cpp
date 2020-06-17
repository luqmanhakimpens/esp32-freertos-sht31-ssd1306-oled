/*
 * main.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Luqman_Hakim
 */

#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "ArduinoJson.h"

#include "serial_debug.h"
#include "tool/tool.h"
#include "network/network.h"
#include "config/config.h"
#include "webserver/webserver.h"
#include "time_manager/time_manager.h"
#include "sensor/sht31.h"
#include "display/display.h"

TaskHandle_t task_handle_sensing;
TaskHandle_t task_handle_clock;
SemaphoreHandle_t xMutex = xSemaphoreCreateMutex();

tool tool;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

config_t config;
int split_line_x = display.width() * 2 / 4 + 5;

void display_time(timeinfo_t timeinfo)
{
	char buff[256];
	int x_offset = split_line_x + 10;

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

void display_sensing(sht_data_t sht)
{
	char buff[256];

	display_erase(0, 0, split_line_x, display.height());

	display_write(0, 0, "temperature");
	sprintf(buff, "%.1fC", sht.temperature);
	display_write(0, 10, buff, 2);

	display_write(0, 35, "humidity");
	sprintf(buff, "%.1f%%", sht.humidity);
	display_write(0, 45, buff, 2);

	display.drawFastVLine(split_line_x, 0, display.height(), WHITE);
	display.display();
}

void task_sensing(void *parameter)
{
	while(1)
	{
		xSemaphoreTake(xMutex, portMAX_DELAY);

		sht_data_t sht;
		sht_read(&sht);
		display_sensing(sht);

		xSemaphoreGive(xMutex);
		vTaskDelay(2000 / portTICK_RATE_MS);
	}
}

void task_clock(void *paramater)
{
	while(1)
	{
		xSemaphoreTake(xMutex, portMAX_DELAY);

		timeinfo_t t;
		time_get(&t);
		display_time(t);

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
	config_init(&config);
	async_webserver_init(&config);
	display_init();
	sht_init();
	wifi_init(&config);
	time_init(NTP_SERVER, GMT_PLUS_7, DAYLIGHT_OFFSET_NONE);
	task_create_clock();
	task_create_sensing();
}

void loop()
{

}
