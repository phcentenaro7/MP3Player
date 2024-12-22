#include "rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "lcd.h"
#include "sdkconfig.h"

void app_main(void)
{
    lcd_init();
    while(true)
    {
        lcd_write("This is a test", 0);
        lcd_write_center("Center!", 1);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        lcd_write("This is a test", 1);
        lcd_write_center("Center!", 0);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    lcd_end();
}