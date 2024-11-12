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

    bool FileSystemStructure::IsFileValid(File file)
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

    bool FileSystemStructure::IsFolderValid(File folder)
    {
        char* name = (char*)folder.name();
        if(strlen(name) != 2) return false;
        return (isdigit(name[0]) && isdigit(name[1]) && (name[0] > '0' || name[1] > '0'));
    }

    uint8_t FileSystemStructure::CountValidFiles(uint8_t folderNumber)
    {
        //Implementation needed
        return 0;
    }

    uint8_t FileSystemStructure::CountValidFolders()
    {
        //Implementation needed
        return 0;
    }

    void FileSystemStructure::LoadFolders()
    {
        //Implementation needed
    }

    FileSystemStructure::FileSystemStructure()
    {
        this->selected_folder = 1;
        for(uint8_t i = 0; i < 99; i++)
        {
            this->folders[i] = NULL;
        }
    }
}