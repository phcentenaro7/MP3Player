#ifndef SD_CARD_HPP
#define SD_CARD_HPP
#include <SD.h>
#include <FS.h>
#include <string.h>
#include <stdint.h>
#include "esp_log.h"
#include "pins.hpp"
#include "misc.hpp"

namespace PlayerSD
{
    bool Initialize();
    uint16_t CountFiles(File& directory, const char* extension = NULL);
    void ListFiles(File& directory);
    void ListDirectories(File& directory);
    void ShowFileTree(File& root, uint8_t level = 0);
}

#endif