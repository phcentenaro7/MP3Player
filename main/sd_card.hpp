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
    class FolderManager
    {
        private:
            PlayerID3::ID3Tag* base;
            uint8_t count;
            bool IsFileValid(File file);
            void AddFile(File file);
            PlayerID3::ID3Tag operator[](uint8_t index);
        public:
            FolderManager() : base(NULL), count(0){};
            FolderManager(uint8_t folderNumber);
            inline uint8_t GetFileCount() {return count;};
            ~FolderManager();
    };
    class FileSystemManager
    {
        private:
            char* root;
            uint8_t selected_folder;
            FolderManager folders[99];
        public:
            FileSystemManager();
    };
}

#endif