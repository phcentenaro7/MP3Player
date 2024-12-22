#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "hd44780.h"
#include "lcd.h"
#include "pcf8574.h"
#include "sdkconfig.h"
#include "string.h"

//static const unsigned char nrows = 2;
#define NCOLS 16

#define DEFAULT_CONTROL {{"", ""}, {"", ""}, false, NULL, NULL}

static struct
{
    char rows[2][NCOLS + 1];
    char queues[2][NCOLS + 1];
    bool icon_mode;
    TaskHandle_t task_handle;
    SemaphoreHandle_t write_mutex;
} control = DEFAULT_CONTROL;

void lcd_init()
{
    control.write_mutex = xSemaphoreCreateMutex();
    xTaskCreate(lcd_task, "LCD Task", 10000, NULL, configMAX_PRIORITIES - 1, &control.task_handle);
}

void lcd_end()
{
    xSemaphoreTake(control.write_mutex, portMAX_DELAY);
    vTaskDelete(control.task_handle);
    vSemaphoreDelete(control.write_mutex);
    pcf8574_end();
}

void lcd_write(char* str, unsigned char row)
{
    xSemaphoreTake(control.write_mutex, portMAX_DELAY);
    memset(control.rows[row], ' ', NCOLS);
    strncpy(control.rows[row], str, NCOLS);
    xSemaphoreGive(control.write_mutex);
}

void lcd_write_center(char* str, unsigned char row)
{
    unsigned char len = strlen(str);
    if(len > 15)
    {
        lcd_write(str, row);
        return;
    }
    xSemaphoreTake(control.write_mutex, portMAX_DELAY);
    memset(control.rows[row], ' ', NCOLS);
    unsigned char col = (NCOLS - len) / 2;
    strncpy(control.rows[row] + col, str, len);
    xSemaphoreGive(control.write_mutex);
}

void lcd_set_icon_mode(bool option)
{
    control.icon_mode = option;
}

void lcd_task(void* params)
{
    pcf8574_init();
    hd44780_init();
    while(true)
    {
        vTaskDelay(CONFIG_LCD_REFRESH_PERIOD / portTICK_PERIOD_MS);
        xSemaphoreTake(control.write_mutex, portMAX_DELAY);
        hd44780_write_string(control.rows[0], 0, 0);
        hd44780_write_string(control.rows[1], 1, 0);
        xSemaphoreGive(control.write_mutex);
    }
}