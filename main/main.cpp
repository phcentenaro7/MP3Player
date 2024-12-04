/** This program tests the SD card module is working */
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "sd_card.hpp"
#include "id3.hpp"
#include "HD44780.h"
#include "esp_log.h"
#include "misc.hpp"

static const char* TAG = "main";

extern "C" void app_main()
{
	ESP_LOGI(TAG, "Starting MP3Player");
	initArduino();
  	Serial.begin(115200);
	while (!Serial) { }

	LCD_init(0x27, PlayerPins::I2C_SDA, PlayerPins::I2C_SCL, 16, 2);
	LCD_clearScreen();
	char folderUp[8] = 
	{
		0b00100,
		0b01010,
		0b01010,
		0b10001,
		0b10001,
		0b00000,
		0b00000,
		0b00000
	};
	LCD_writeToCGRAM(0, folderUp);
	while(true)
	{
		LCD_setDisplayOn();
		LCD_setCursor(0, 0);
		LCD_writeChar(0);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
		LCD_setDisplayOff();
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}