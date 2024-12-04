/** This program tests the SD card module is working */
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "sd_card.hpp"
#include "id3.hpp"
#include "HD44780.h"
#include "lcd.hpp"
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
	LCD_setDisplayOn();
	for(int i = 0; i < 8; i++)
	{
		LCD_writeToCGRAM(i << 3, PlayerLCD::specialChars[i]);
		LCD_writeChar(i);
	}
	while(true)
	{
		LCD_setCursor(0, 1);
		for(int i = 0; i < 5; i++)
		{
			LCD_writeChar(i);
			LCD_writeChar(' ');
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}