#include "hd44780.h"
#include "pcf8574.h"
#include "rom/ets_sys.h"
#include "sdkconfig.h"

static char* TAG = "LCD";

static const char reg_rs = 0x01;
//static const char reg_rw = 0x02;
static const char reg_e = 0x04;
static const char backlight = 0x08;

/*
 * Initializes communication with the HD44780 display.
 * Consult page 46, figure 24 of the HD44780 datasheet for more information.
 * The following settings are applied:
 * - 4-bit communication mode;
 * - 5x8 characters;
 * - 2 rows and 16 columns;
 * - Display initially on, with backlight and no cursor;
 * - No display shift.
 */
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
    hd44780_on();
}

/*
 * Sends a byte of data to the HD44780.
 * This process actually sends two bytes of data. The first byte sends the data with the Enable register activated, signaling to the HD44780 that new data is coming.
 * The second byte is sent 1 us after the first, and is identical to the first, but the Enable register is deactivated. This process is necessary to communicate using the HD44780 is 4-bit mode with the PCF8574.
 * For more information, consult page 33, figure 17 of the HD44780 datasheet.
 */
void hd44780_send_byte(char byte)
{
    pcf8574_send_byte(byte | reg_e | backlight);
    ets_delay_us(1);
    pcf8574_send_byte(byte | backlight);
}

/*
 * Sends each nibble of a byte to the HD44780.
 * This function begins by sending the most significant nibble, then the least significant. After the first nibble is sent, it waits for 1 us. After the second nibble is sent, it waits for 50 us. This is because most commands should take at most 37 us to execute (page 24, table 6 of the HD44780 datasheet).
 */
void hd44780_send_nibbles(char byte, char regs)
{
    hd44780_send_byte((byte & 0xF0) | regs);
    ets_delay_us(1);
    hd44780_send_byte((byte << 4) | regs);
    ets_delay_us(50);
}

// Clears the display.
void hd44780_clear()
{
    hd44780_send_nibbles(0x01, 0);
    ets_delay_us(2e3);
}

// Returns the cursor home, and returns the display to the original position.
void hd44780_home()
{
    hd44780_send_nibbles(0x02, 0);
    ets_delay_us(2e3);
}

// Sets the cursor to move to the right and disables display shift.
void hd44780_entry_mode()
{
    hd44780_send_nibbles(0x06, 0);
}

// Disables the display from showing characters. 
void hd44780_off()
{
    hd44780_send_nibbles(0x08, 0);
}

// Enables the display to show characters.
void hd44780_on()
{
    hd44780_send_nibbles(0x0C, 0);
}

/* Initialization-only function. Configures the display according to the following:
 * - 4-bit communication mode;
 * - 5x8 characters;
 * - 2 rows and 16 columns.
 */
void hd44780_function_1602()
{
    hd44780_send_nibbles(0x28, 0);
}

/*
 * Sets the current CGRAM address. Used to write custom characters.
 * Valid address values for the 5x8 character pattern displays are 0 to 7. Any address outside of this range will result in an error.
 */
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

/*
 * Sets the current DDRAM address. Used to write text to the screen.
 * Valid address values are 0x00 to 0x27 and 0x40 to 0x67. Any address outside of this range will result in an error.
 */
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

/*
 * Writes a character from CGRAM to the designated row and column of the HD44780.
 * Valid rows are 0 or 1, and valid columns are 0 to 15. Any values outside of this range will result in an error.
 */
void hd44780_write_char(char ch, unsigned char row, unsigned char col)
{
    ESP_ERROR_CHECK(hd44780_set_ddram_address(0x40 * row + col));
    hd44780_send_nibbles(ch, reg_rs);
}

/*
 * Writes a string of `n` characters from CGRAM to the HD44780, starting at the designated row and column.
 * This function wraps around to the first column of the other row, if the string is too big for the current row.
 * Valid initial rows are 0 or 1, and valid initial columns are 0 to 15. Any values outside of this range will result in an error.
 */
void hd44780_write_nchars(char* ch, unsigned char n, unsigned char row, unsigned char col)
{
    for(unsigned char i = 0; i < n; i++)
    {
        hd44780_write_char(ch[i], row, col);
        if(++col > 15)
        {
            col = 0;
            row ^= 1;
        }
    }
}

/*
 * Writes a string of characters from CGRAM to the HD44780, starting at the designated row and column.
 * This function wraps around to the first column of the other row, if the string is too big for the current row.
 * Valid initial rows are 0 or 1, and valid initial columns are 0 to 15. Any values outside of this range will result in an error.
 */
void hd44780_write_string(char* str, unsigned char row, unsigned char col)
{
    while(*str != '\0')
    {
        hd44780_write_char(*str, row, col);
        if(++col > 15)
        {
            col = 0;
            row ^= 1;
        }
        str++;
    }
}

/*
 * Writes a new custom character to CGRAM.
 * Valid address values for the 5x8 character pattern displays are 0 to 7. Any address outside of this range will result in an error.
 */
void hd44780_new_char(unsigned char* pattern, unsigned char addr)
{
    addr <<= 3;
    for(unsigned char row = 0; row < 8; row++)
    {
        ESP_ERROR_CHECK(hd44780_set_cgram_address(addr | row));
        hd44780_send_nibbles(pattern[row], reg_rs);
    }
}