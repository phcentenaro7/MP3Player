#ifndef PCF8574_H
#define PCF8574_H

void pcf8574_init();
void pcf8574_send_byte(unsigned char byte);
void pcf8574_end();

#endif