#include "include/PCF8574.h"
#include "driver/i2c_master.h"

i2c_master_dev_handle_t PCF8574_init(gpio_num_t sda_pin, gpio_num_t scl_pin, unsigned int scl_speed_hz, unsigned int dev_addr)
{
    i2c_master_bus_config_t i2c_master_config = 
    {
        .i2c_port = -1,
        .sda_io_num = sda_pin,
        .scl_io_num = scl_pin,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true
    };
    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_master_config, &bus_handle));
    i2c_device_config_t dev_config = 
    {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = dev_addr,
        .scl_speed_hz = scl_speed_hz
    };
    i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_config, &dev_handle));
    return dev_handle;
}