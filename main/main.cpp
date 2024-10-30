/** This program tests the SD card module is working */
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "sd_card.hpp"
#include "id3.hpp"
#include "esp_log.h"
#include "misc.hpp"

static const char* TAG = "main";

extern "C" void app_main()
{
	ESP_LOGI(TAG, "Starting MP3Player");
	initArduino();
  	Serial.begin(9600);
	while (!Serial) { }

	PlayerSD::Initialize();
	File root = SD.open("/");
	PlayerSD::ShowFileTree(root);
	File file;
	while(true)
	{
		file = root.openNextFile();
		if(!file.isDirectory())
		{
			break;
		}
		file.close();
	}
	if(PlayerID3::FileHasID3Tag(file))
	{
		printf("File %s has ID3 tag\n", file.name());
		PlayerID3::ID3Header header = PlayerID3::ID3Header(file);
		printf("major version: %d\n", header.major_version);
		printf("revision number: %d\n", header.revision_number);
		printf("flags: %d\n", header.flags);
		printf("size: %ld\n", header.size);
		PlayerMisc::SwitchEndianness(header.size);
		PlayerMisc::UndoSynchsafe(header.size);
		printf("switched size: %ld\n", header.size);
	}
	file.close();
}
