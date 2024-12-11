#include "freertos/FreeRTOS.h"
#include "PCF8574.h"
#include "sdkconfig.h"

void app_main(void)
{
    PCF8574_init(CONFIG_PCF8574_SDA_PIN, CONFIG_PCF8574_SCL_PIN, CONFIG_PCF8574_SCL_SPEED_HZ, CONFIG_PCF8574_DEV_ADDR);
}