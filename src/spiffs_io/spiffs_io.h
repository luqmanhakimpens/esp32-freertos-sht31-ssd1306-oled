/*
 * spiffs_io.h
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */

#ifndef SRC_SPIFFS_IO_SPIFFS_IO_H_
#define SRC_SPIFFS_IO_SPIFFS_IO_H_


#include "SPIFFS.h"

String spiffs_read(fs::FS &fs, const char * path);
void spiffs_write(fs::FS &fs, const char * path, const char * message);
void spiffs_delete(fs::FS &fs, const char * path);

#endif /* SRC_SPIFFS_IO_SPIFFS_IO_H_ */
