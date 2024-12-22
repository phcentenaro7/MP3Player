#ifndef LCD_H
#define LCD_H

void lcd_init();
void lcd_end();

void lcd_write(char* str, unsigned char row);
void lcd_write_center(char* str, unsigned char row);
void lcd_set_icon_mode(bool option);

void lcd_task(void* params);

#endif