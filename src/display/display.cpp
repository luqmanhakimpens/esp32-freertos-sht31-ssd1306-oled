/*
 * display.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#include "display.h"

extern Adafruit_SSD1306 display;

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

void display_write(const char *text, uint8_t text_size)
{
	if(text_size != 1) display.setTextSize(text_size);
	display.println(text);
	if(text_size != 1) display.setTextSize(1);
}

void display_write(int pos_x, int pos_y, const char *text, uint8_t text_size)
{
	display.setCursor(pos_x, pos_y);
	display_write(text, text_size);
}

void display_erase(int x, int y, int w, int h)
{
	display.fillRect(x, y, w, h, BLACK);
}
