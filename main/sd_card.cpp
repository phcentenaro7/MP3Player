#include "sd_card.hpp"

static const char* TAG = "sd_card";

namespace PlayerSD
{
    bool Initialize()
    {
        ESP_LOGI(TAG, "Initializing SD card");
        if(!SD.begin(PlayerPins::SPI_CS))
        {
            ESP_LOGE(TAG, "SD card initialization failed");
            return false;
        }
        return true;
    }

    uint16_t CountFiles(File& directory, const char* extension)
    {
        ESP_LOGI(TAG, "Counting SD files");
        uint16_t count = 0;
        while(true)
        {
            File handle = directory.openNextFile();
            if(!handle)
            {
                ESP_LOGI(TAG, "Finished counting files, %hd in total", count);
                directory.rewindDirectory();
                return count;
            }
            if(!handle.isDirectory())
            {
                if(!extension || PlayerMisc::StringEndsWith(handle.name(), extension))
                {
                    count++;
                }
            }
        }
    }

    void ListFiles(File& directory)
    {
        while(true)
        {
            File handle = directory.openNextFile();
            if(!handle)
            {
                directory.rewindDirectory();
                return;
            }
            if(!handle.isDirectory())
            {
                printf("%s\n", handle.name());
            }
        }
    }

    void ListDirectories(File& directory)
    {
        while(true)
        {
            File handle = directory.openNextFile();
            if(!handle)
            {
                directory.rewindDirectory();
                return;
            }
            if(handle.isDirectory())
            {
                printf("%s\n", handle.name());
            }
        }
    }

    void ShowFileTree(File& root, uint8_t level)
    {
        while(true)
        {
            File handle = root.openNextFile();
            if(!handle)
            {
                root.rewindDirectory();
                return;
            }
            for(uint8_t i = 0; i < level; i++)
            {
                printf("\t");
            }
            printf("%s\n", handle.name());
            if(handle.isDirectory())
            {
                uint16_t next_level = level + 1;
                File next_root = handle;
                ShowFileTree(next_root, next_level);
            }
        }
    }

    FolderManager::FolderManager(uint8_t folderNumber) : FolderManager()
    {
        if (folderNumber > 99) return;
        char folderName[4] = "";
        sprintf(folderName, "/%01d", folderNumber);
        if(!SD.exists(folderName)) return;
        File folder = SD.open(folderName);
        File file = folder.openNextFile();
        while(this->count < 255 && file)
        {
            if(!file.isDirectory() && IsFileValid(file))
            {
                AddFile(file);
            }
            file.close();
            file = folder.openNextFile();
        }
    }

    bool FolderManager::IsFileValid(File file)
    {
        char* name = (char*)file.name();
        if(strlen(name) != 7 || !PlayerMisc::StringEndsWith(name, ".MP3")) return false;
        bool nonZero = false;
        for(size_t i = 0; i < 3; i++)
        {
            if(!isdigit(name[i])) return false;
            if(name[i] > '0') nonZero = true;
        }
        return nonZero;
    }

    void FolderManager::AddFile(File file)
    {
        if(this->count == 0)
        {
            this->base = new PlayerID3::ID3Tag(file);
            this->count++;
            return;
        }
        PlayerID3::ID3Tag* tag = base;
        while(tag->next)
        {
            tag = tag->next;
        }
        tag->next = new PlayerID3::ID3Tag(file);
        this->count++;
    }

    PlayerID3::ID3Tag FolderManager::operator[](uint8_t index)
    {
        PlayerID3::ID3Tag* tag = this->base;
        while(index > 0)
        {
            tag = tag->next;
            index--;
        }
        return *tag;
    }

    FolderManager::~FolderManager()
    {
        PlayerID3::ID3Tag* currentFile = this->base;
        PlayerID3::ID3Tag* nextFile = NULL;
        while(currentFile)
        {
            nextFile = currentFile->next;
            delete currentFile;
            currentFile = nextFile;
        }
    }

    FileSystemManager::FileSystemManager()
    {
        char path[4] = "";
        for(uint8_t i = 1; i <= 99; i++)
        {
            sprintf(path, "/%01d", i);
            if(SD.exists(path)) this->folders[i] = FolderManager(i);
            else this->folders[i] = FolderManager();
        }
    }
}