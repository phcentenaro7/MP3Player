#ifndef SD_CARD_HPP
#define SD_CARD_HPP
#include <SD.h>
#include <FS.h>
#include <string.h>
#include <stdint.h>
#include "esp_log.h"
#include "HD44780.h"
#include "lcd.hpp"
#include "pins.hpp"
#include "misc.hpp"
#include "id3.hpp"

namespace PlayerSD
{
    bool Initialize();
    void ListFiles(File& directory);
    void ListDirectories(File& directory);
    void ShowFileTree(File& root, uint8_t level = 0);
    class FolderManager
    {
        private:
            uint8_t number;
            PlayerID3::ID3Tag* base;
            uint8_t count;
            bool IsFileValid(File file);
            void AddFile(File file);
        public:
            FolderManager() : number(0), base(NULL), count(0){};
            void Load(uint8_t folderNumber, PlayerLCD::LCD* lcd = NULL);
            inline uint8_t GetFileCount() {return count;};
            PlayerID3::ID3Tag& GetFile(uint8_t index);
            inline PlayerID3::ID3Tag& operator[](uint8_t index) {return GetFile(index);};
            void Print();
            ~FolderManager();
    };
    class FileSystemManager
    {
        private:
            const char* root;
            uint8_t selected_folder;
            FolderManager folders[99];
        public:
            FileSystemManager(PlayerLCD::LCD* lcd = NULL);
            FolderManager& GetFolder(uint8_t index);
            inline FolderManager& operator[](uint8_t index) {return GetFolder(index);};
            void Print();
    };
}

#endif