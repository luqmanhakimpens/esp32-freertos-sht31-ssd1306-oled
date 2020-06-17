/*
 * display.h
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#ifndef SRC_DISPLAY_DISPLAY_H_
#define SRC_DISPLAY_DISPLAY_H_

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Fonts/FreeMonoBold9pt7b.h"
#include "Fonts/FreeSerif9pt7b.h"
#include "app_config.h"

bool display_init();
void display_write(const char *text, uint8_t text_size = 1);
void display_write(int pos_x, int pos_y, const char *text, uint8_t text_size = 1);
void display_erase(int x, int y, int w, int h);

#endif /* SRC_DISPLAY_DISPLAY_H_ */
