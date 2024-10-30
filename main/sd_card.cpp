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
        ESP_LOGI(TAG, "SD card initialization successful");
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
}