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
#include "pins.hpp"
#include "misc.hpp"

static const char* TAG = "main";
static bool readerDone = false;
static bool playerMounted = false;
PlayerSD::FileSystemManager fileSystem;


void ReaderTask(void* param);
void PlayerTask(void* param);
TaskHandle_t readerTaskHandle;
TaskHandle_t playerTaskHandle;

SemaphoreHandle_t displayMutex = xSemaphoreCreateMutex();

extern "C" void app_main()
{
	ESP_LOGI(TAG, "Starting MP3Player");
	initArduino();
  	Serial.begin(115200);
	while(!Serial){};

	LCD_init(PlayerLCD::LCD_ADDRESS, PlayerPins::I2C_SDA, PlayerPins::I2C_SCL, PlayerLCD::NUM_COLUMNS, PlayerLCD::NUM_ROWS);
	LCD_clearScreen();

	xTaskCreate(ReaderTask, "ReaderTask", 100000, NULL, 0, &readerTaskHandle);
	//xTaskCreate(PlayerTask, "PlayerTask", 10000, NULL, 0, &playerTaskHandle);

	pinMode(PlayerPins::BUTTON_CHOICE_1, INPUT_PULLDOWN);
	pinMode(PlayerPins::BUTTON_CHOICE_2, INPUT_PULLDOWN);
	pinMode(PlayerPins::BUTTON_SWITCH, INPUT_PULLDOWN);

	while(true)
	{
		xSemaphoreTake(displayMutex, portMAX_DELAY);
		LCD_home();
		LCD_clearScreen();
		if(!readerDone)
		{
			LCD_writeCentered("Insert SD card", 0);
			LCD_writeCentered("in reader!", 1);
		}
		else
		{
			LCD_writeCentered("Ready to play!", 0);
		}
		xSemaphoreGive(displayMutex);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void ReaderTask(void* param)
{
	while(true)
	{
		while(readerDone || !PlayerSD::Initialize()){};
		xSemaphoreTake(displayMutex, portMAX_DELAY);
		LCD_home();
		LCD_clearScreen();
		LCD_writeCentered("SD inserted!", 0);
		vTaskDelay(3000 / portTICK_PERIOD_MS);
		LCD_clearScreen();
		LCD_writeCentered("Loading...", 0);
		fileSystem = PlayerSD::FileSystemManager();
		readerDone = true;
		xSemaphoreGive(displayMutex);
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}

void PlayerTask(void* param)
{
	while(true)
	{
		while(!readerDone)
		{
			xSemaphoreTake(displayMutex, portMAX_DELAY);

		}
	}
}