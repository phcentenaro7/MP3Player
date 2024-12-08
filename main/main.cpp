/** This program tests the SD card module is working */
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "system.hpp"
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
static PlayerSD::FileSystemManager* fileSystem = NULL;
static PlayerLCD::LCD* lcd = NULL;
static PlayerSystem::System* sys = NULL;

void AwaitCardInReader();
void RetrieveCardData();
void AwaitCardInPlayer();

extern "C" void app_main()
{
	esp_log_level_set("*", ESP_LOG_DEBUG);
	ESP_LOGI(TAG, "Starting MP3Player");
	initArduino();
  	Serial.begin(115200);
	while(!Serial){};

	lcd = new PlayerLCD::LCD();

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
	lcd->Write("Insert SD card", 0);
	lcd->Write("in reader!", 1);
	while(!PlayerSD::Initialize()){};
}

void RetrieveCardData()
{
	lcd->Write("SD inserted!", 0);
	lcd->Write("", 1);
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	lcd->Write("Loading...", 0);
	fileSystem = new PlayerSD::FileSystemManager(lcd);
	//sys = new PlayerSystem::System(fileSystem);
	readerDone = true;
}

void AwaitCardInPlayer()
{
	lcd->Write("Ready to play!", 0);
	lcd->Write("", 1);
	vTaskDelay(10000 / portTICK_PERIOD_MS);
	sys = new PlayerSystem::System(fileSystem, lcd);
	lcd->Write("Playing!", 0);
	while(true)
	{
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	// DF_volume(30);
	// vTaskDelay(100 / portTICK_PERIOD_MS);
	// DF_playFolder(2, 1);
	// while(true)
	// {
	// 	vTaskDelay(5000 / portTICK_PERIOD_MS);
	// 	DF_playFolder(2, 1);
	// }
}