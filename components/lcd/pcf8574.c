#include "pcf8574.h"
#include "driver/i2c_master.h"
#include "sdkconfig.h"

i2c_master_bus_handle_t pcf8574_bus_handle;
i2c_master_dev_handle_t pcf8574_dev_handle;

void pcf8574_init()
{
    i2c_master_bus_config_t i2c_master_config = 
    {
        .i2c_port = -1,
        .sda_io_num = CONFIG_PCF8574_SDA_PIN,
        .scl_io_num = CONFIG_PCF8574_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_master_config, &pcf8574_bus_handle));
    i2c_device_config_t dev_config = 
    {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = CONFIG_PCF8574_DEV_ADDR,
        .scl_speed_hz = CONFIG_PCF8574_SCL_SPEED_HZ
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(pcf8574_bus_handle, &dev_config, &pcf8574_dev_handle));
}

void pcf8574_send_byte(char byte)
{
    unsigned char b = byte;
    ESP_ERROR_CHECK(i2c_master_transmit(pcf8574_dev_handle, &b, 1, -1));
}

void pcf8574_end()
{
    ESP_ERROR_CHECK(i2c_master_bus_rm_device(pcf8574_dev_handle));
    ESP_ERROR_CHECK(i2c_del_master_bus(pcf8574_bus_handle));
}