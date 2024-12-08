#include "lcd.hpp"

static const char* TAG = "LCD";
static char rows[2][17] = {"", ""};
static char queuedRows[2][17] = {"", ""};
static char icons[4] = {' ', ' ', ' ', ' '};
static SemaphoreHandle_t writeMutex;
static TaskHandle_t awaitRefreshHandle;
static TimerHandle_t refreshHandle;
static bool refresh = false;
static bool iconMode = false;

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
            for(uint8_t i = 0; i < 2; i++)
            {
                if(strcmp(rows[i], queuedRows[i]))
                {
                    LCD_setCursor(0, i);
                    if(strlen(rows[i]) != strlen(queuedRows[i]))
                    {
                        LCD_writeStr("                ");
                    }
                    strcpy(rows[i], queuedRows[i]);
                    if(iconMode && i == 1)
                    {
                        LCD_setCursor(0, 1);
                        for(uint8_t i = 0; i < 4; i++)
                        {
                            LCD_writeChar(icons[i]);
                        }
                        LCD_writeStr(rows[i]);
                    }
                    else
                    {
                        LCD_writeCentered(rows[i], i);
                    }
                }
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
        LoadSpecialCharacters();
        writeMutex = xSemaphoreCreateMutex();
        refreshHandle = xTimerCreate("LCD Refresh Timer", pdMS_TO_TICKS(refreshPeriodMs), pdTRUE, (void*)0, Tick);
        xTimerStart(refreshHandle, 0);
        xTaskCreate(AwaitRefresh, "LCD Await Refresh", 10000, NULL, 0, &awaitRefreshHandle);
    }

    void LCD::SetIconMode(bool value)
    {
        iconMode = value;
    }

    void LCD::LoadSpecialCharacters()
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            LCD_writeToCGRAM(i << 3, specialChars[i]);
        }
    }

    void LCD::Write(const char* text, uint8_t row)
    {
        xSemaphoreTake(writeMutex, portMAX_DELAY);
        strcpy(queuedRows[row], text);
        xSemaphoreGive(writeMutex);
    }

    void LCD::SetIcons(uint8_t icon1, uint8_t icon2, uint8_t icon3, uint8_t icon4)
    {
        icons[0] = icon1;
        icons[1] = icon2;
        icons[2] = icon3;
        icons[3] = icon4;
    }
}