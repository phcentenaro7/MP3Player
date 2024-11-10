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
	File f = SD.open("/001.MP3");
	PlayerID3::ID3Tag tag = PlayerID3::ID3Tag(f);
	tag.Print();
	f.close();
}
