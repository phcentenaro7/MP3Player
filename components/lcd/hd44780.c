#include "esp_check.h"
#include "hd44780.h"
#include "pcf8574.h"
#include "rom/ets_sys.h"
#include "sdkconfig.h"

static char* TAG = "LCD";

static const char reg_rs = 0x01;
//static const char reg_rw = 0x02;
static const char reg_e = 0x04;
static const char backlight = 0x08;

void hd44780_init()
{
    pcf8574_send_byte(0);
    ets_delay_us(100e3);
    hd44780_send_byte(0x30);
    ets_delay_us(4.5e3);
    hd44780_send_byte(0x30);
    ets_delay_us(150);
    hd44780_send_byte(0x30);
    ets_delay_us(50);
    hd44780_send_byte(0x20);
    ets_delay_us(50);
    hd44780_function_1602();
    hd44780_off();
    hd44780_entry_mode();
    hd44780_clear();
}

void hd44780_send_byte(char byte)
{
    pcf8574_send_byte(byte | reg_e | backlight);
    ets_delay_us(1);
    pcf8574_send_byte(byte | backlight);
}

void hd44780_send_nibbles(char byte, char regs)
{
    hd44780_send_byte((byte & 0xF0) | regs);
    ets_delay_us(50);
    hd44780_send_byte((byte << 4) | regs);
    ets_delay_us(50);
}

void hd44780_clear()
{
    hd44780_send_nibbles(0x01, 0);
    ets_delay_us(2e3);
}

void hd44780_home()
{
    hd44780_send_nibbles(0x02, 0);
    ets_delay_us(2e3);
}

void hd44780_entry_mode()
{
    hd44780_send_nibbles(0x06, 0);
}

void hd44780_off()
{
    hd44780_send_nibbles(0x08, 0);
}

void hd44780_on()
{
    hd44780_send_nibbles(0x0C, 0);
}

void hd44780_function_1602()
{
    hd44780_send_nibbles(0x28, 0);
}

esp_err_t hd44780_set_cgram_address(unsigned char addr)
{
    if(addr > 0x07)
    {
        ESP_LOGE(TAG, "Attempt to set CGRAM address to invalid value 0x%x. Valid interval is 0x00 <= addr <= 0x07.", addr);
        return ESP_ERR_INVALID_ARG;
    }
    char addr_comm = 0x40 | addr;
    hd44780_send_nibbles(addr_comm, 0);
    return ESP_OK;
}

esp_err_t hd44780_set_ddram_address(unsigned char addr)
{
    if((addr > 0x27 && addr < 0x40) || (0x67 < addr))
    {
        ESP_LOGE(TAG, "Attempt to set DDRAM address to invalid value 0x%x. Valid intervals are 0x00 <= addr <= 0x27 and 0x40 <= addr <= 0x67.", addr);
        return ESP_ERR_INVALID_ARG;
    }
    char addr_comm = 0x80 | addr;
    hd44780_send_nibbles(addr_comm, 0);
    return ESP_OK;
}

void hd44780_write_char(char ch, unsigned char row, unsigned char col)
{
    ESP_ERROR_CHECK(hd44780_set_ddram_address(0x40 * row + col));
    hd44780_send_nibbles(ch, reg_rs);
}

void hd44780_write_string(char* str, unsigned char row, unsigned char col)
{
    while(*str != '\0')
    {
        hd44780_write_char(*str, row, col);
        if(++col > 39)
        {
            col = 0;
            row ^= 1;
        }
        str++;
    }
}

void hd44780_new_char(unsigned char* pattern, unsigned char addr)
{
    addr <<= 3;
    for(unsigned char row = 0; row < 8; row++)
    {
        ESP_ERROR_CHECK(hd44780_set_cgram_address(addr | row));
        hd44780_send_nibbles(pattern[row], reg_rs);
    }
}