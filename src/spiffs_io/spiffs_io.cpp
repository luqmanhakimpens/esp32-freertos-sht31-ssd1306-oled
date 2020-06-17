/*
 * spiffs_io.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: luqma
 */


#include "spiffs_io.h"

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
