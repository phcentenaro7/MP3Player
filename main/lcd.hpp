#include <stdint.h>

namespace PlayerLCD
{
    const uint8_t CHAR_FOLDER_INC = 0;
    const uint8_t CHAR_FOLDER_DEC = 1;
    const uint8_t CHAR_TRACK_INC = 2;
    const uint8_t CHAR_TRACK_DEC = 3;
    const uint8_t CHAR_VOLUME_INC = 4;
    const uint8_t CHAR_VOLUME_DEC = 5;
    const uint8_t CHAR_PLAY = 6;
    const uint8_t CHAR_PAUSE = 7;
    char specialChars[8][8] = 
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
}