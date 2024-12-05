#include "lcd.hpp"

static const char* TAG = "LCD";
static char rows[2][17] = {"", ""};
static char queuedRows[2][17] = {"", ""};
static SemaphoreHandle_t writeMutex;
static TaskHandle_t awaitRefreshHandle;
static TimerHandle_t refreshHandle;
static bool refresh = false;

static void Tick(TimerHandle_t timerHandle)
{
    refresh = true;
}

static void AwaitRefresh(void* param)
{
    while(true)
    {
        if(refresh)
        {
            xSemaphoreTake(writeMutex, portMAX_DELAY);
            bool rewrite = false;
            if(strcmp(rows[0], queuedRows[0]))
            {
                strcpy(rows[0], queuedRows[0]);
                rewrite = true;
            }
            if(strcmp(rows[1], queuedRows[1]))
            {
                strcpy(rows[1], queuedRows[1]);
                rewrite = true;
            }
            if(rewrite)
            {
                LCD_home();
                LCD_setCursor(0, 0);
                LCD_clearScreen();
                LCD_writeCentered(rows[0], 0);
                LCD_writeCentered(rows[1], 1);
            }
            refresh = false;
            xSemaphoreGive(writeMutex);
        }
    }
}

namespace PlayerLCD
{
    LCD::LCD(uint8_t address, uint8_t sdaPin, uint8_t sclPin, size_t refreshPeriodMs)
    {
        strcpy(rows[0], "");
        strcpy(rows[1], "");
        strcpy(queuedRows[0], "");
        strcpy(queuedRows[1], "");
        refresh = false;
        LCD_init(address, sdaPin, sclPin, 16, 2);
        writeMutex = xSemaphoreCreateMutex();
        refreshHandle = xTimerCreate("LCD Refresh Timer", pdMS_TO_TICKS(refreshPeriodMs), pdTRUE, (void*)0, Tick);
        xTimerStart(refreshHandle, 0);
        xTaskCreate(AwaitRefresh, "LCD Await Refresh", 10000, NULL, 0, &awaitRefreshHandle);
    }

    void LCD::Write(const char* upper_row, const char* lower_row)
    {
        xSemaphoreTake(writeMutex, portMAX_DELAY);
        if(upper_row) strcpy(queuedRows[0], upper_row);
	    if(lower_row) strcpy(queuedRows[1], lower_row);
        xSemaphoreGive(writeMutex);
    }
}