#ifndef SD_CARD_HPP
#define SD_CARD_HPP
#include <SD.h>
#include <FS.h>
#include <string.h>
#include <stdint.h>
#include "esp_log.h"
#include "pins.hpp"
#include "misc.hpp"
#include "id3.hpp"

namespace PlayerSD
{
    bool Initialize();
    void ListFiles(File& directory);
    void ListDirectories(File& directory);
    void ShowFileTree(File& root, uint8_t level = 0);
    class FileSystemStructure
    {
        private:
            uint8_t selected_folder;
            PlayerID3::ID3Tag* folders[99];
            bool IsFolderValid(File folder);
            bool IsFileValid(File file);
            uint8_t CountValidFolders();
            uint8_t CountValidFiles(uint8_t folderNumber);
            void LoadFolders();
        public:
            FileSystemStructure();
    };
}

#endif