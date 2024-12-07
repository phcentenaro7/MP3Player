/** This program tests the SD card module is working */
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "sd_card.hpp"
#include "id3.hpp"
#include "HD44780.h"
#include "DFRobotDFPlayerMini.h"
#include "lcd.hpp"
#include "esp_log.h"
#include "pins.hpp"
#include "misc.hpp"

static const char* TAG = "main";
static bool readerDone = false;
static PlayerSD::FileSystemManager* fileSystem;
static PlayerLCD::LCD* lcd;
    
void AwaitCardInReader();
void RetrieveCardData();
void AwaitCardInPlayer();

extern "C" void app_main()
{
	ESP_LOGI(TAG, "Starting MP3Player");
	initArduino();
  	Serial.begin(115200);
	while(!Serial){};
	while(!DF_begin(PlayerPins::SERIAL_TX, PlayerPins::SERIAL_RX, true, true, true))
	{
		puts("Trying again");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	};

	lcd = new PlayerLCD::LCD();

	pinMode(PlayerPins::BUTTON_CHOICE_1, INPUT_PULLDOWN);
	pinMode(PlayerPins::BUTTON_CHOICE_2, INPUT_PULLDOWN);
	pinMode(PlayerPins::BUTTON_SWITCH, INPUT_PULLDOWN);

	while(true)
	{
		LCD_home();
		LCD_clearScreen();
		if(!readerDone)
		{
			AwaitCardInReader();
			RetrieveCardData();
		}
		else
		{
			AwaitCardInPlayer();
		}
	}
}

void AwaitCardInReader()
{
	lcd->Write("Insert SD card", "in reader!");
	while(!PlayerSD::Initialize()){};
}

void RetrieveCardData()
{
	lcd->Write("SD inserted!", "");
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	lcd->Write("Loading...", NULL);
	fileSystem = new PlayerSD::FileSystemManager(lcd);
	readerDone = true;
}

void AwaitCardInPlayer()
{
	lcd->Write("Ready to play!", "");
	vTaskDelay(10000 / portTICK_PERIOD_MS);
	lcd->Write("Playing!", NULL);
	DF_volume(30);
	playNext:
	vTaskDelay(1000);
	DF_next();
	vTaskDelay(1000);
	int finished = 0;
	while(!DF_isFinished(&finished))
	{
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	goto playNext;
}