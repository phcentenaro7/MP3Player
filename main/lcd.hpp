#ifndef LCD_HPP
#define LCD_HPP

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "HD44780.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "pins.hpp"
#include "esp_log.h"
#define LCD_NUM_COLUMNS 16
#define LCD_NUM_ROWS 2

namespace PlayerLCD
{
    const uint8_t LCD_ADDRESS = 0x27;
    const uint8_t CHAR_FOLDER_INC = 0;
    const uint8_t CHAR_FOLDER_DEC = 1;
    const uint8_t CHAR_TRACK_INC = 2;
    const uint8_t CHAR_TRACK_DEC = 3;
    const uint8_t CHAR_VOLUME_INC = 4;
    const uint8_t CHAR_VOLUME_DEC = 5;
    const uint8_t CHAR_PLAY = 6;
    const uint8_t CHAR_PAUSE = 7;
    const char specialChars[8][8] = 
    {
        //CHAR_FOLDER_INC (^)
        {
            0b00100,
            0b01010,
            0b01010,
            0b10001,
            0b10001,
            0b00000,
            0b00000,
            0b00000
        },
        //CHAR_FOLDER_DEC (v)
        {
            0b10001,
            0b10001,
            0b01010,
            0b01010,
            0b00100,
            0b00000,
            0b00000,
            0b00000
        },
        //CHAR_TRACK_INC (>)
        {
            0b01000,
            0b00100,
            0b00010,
            0b00001,
            0b00010,
            0b00100,
            0b01000,
            0b00000
        },
        //CHAR_TRACK_DEC (<)
        {
            0b00010,
            0b00100,
            0b01000,
            0b10000,
            0b01000,
            0b00100,
            0b00010,
            0b00000
        },
        //CHAR_TRACK_VOLUME_INC (+)
        {
            0b00100,
            0b00100,
            0b11111,
            0b00100,
            0b00100,
            0b00000,
            0b00000,
            0b00000
        },
        //CHAR_TRACK_VOLUME_DEC (-)
        {
            0b00000,
            0b00000,
            0b11111,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000
        },
        //CHAR_PLAY (|>)
        {
            0b11000,
            0b11100,
            0b11110,
            0b11111,
            0b11110,
            0b11100,
            0b11000,
            0b00000
        },
        //CHAR_PAUSE (||)
        {
            0b01010,
            0b01010,
            0b01010,
            0b01010,
            0b01010,
            0b01010,
            0b01010,
            0b00000
        }
    };
    class LCD
    {
        public:
            void Write(const char* upper_row, const char* lower_row);
            LCD(uint8_t address = PlayerLCD::LCD_ADDRESS, uint8_t sdaPin = PlayerPins::I2C_SDA, uint8_t sclPin = PlayerPins::I2C_SCL, size_t refreshPeriodMs = 20);
    };
}

#endif