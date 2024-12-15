#ifndef HD44780_H
#define HD44780_H

void hd44780_init();

void hd44780_send_byte(char byte);
void hd44780_send_nibbles(char byte, char regs);
void hd44780_clear();
void hd44780_home();
void hd44780_entry_mode();
void hd44780_off();
void hd44780_on();
void hd44780_function_1602();
esp_err_t hd44780_set_ddram_address(unsigned char addr);
void hd44780_write_char(char ch, unsigned char row, unsigned char col);
void hd44780_write_string(char* str, unsigned char row, unsigned char col);

#endif