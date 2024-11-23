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
  	Serial.begin(115200);
	while (!Serial) { }

	PlayerSD::Initialize();
	PlayerSD::FileSystemManager manager = PlayerSD::FileSystemManager();
	manager[0].Print();
	manager[1].Print();
	manager[2].Print();
}