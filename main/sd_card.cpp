#include "sd_card.hpp"

static const char* TAG = "SD";

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

    void FolderManager::Load(uint8_t folderNumber, PlayerLCD::LCD* lcd)
    {
        if (folderNumber > 99) return;
        this->number = folderNumber;
        this->count = 0;
        this->base = NULL;
        char folderName[4] = "";
        sprintf(folderName, "/%02d", folderNumber);
        if(!SD.exists(folderName)) return;
        File folder = SD.open(folderName);
        File file = folder.openNextFile();
        while(this->count < 255 && file)
        {
            if(!file.isDirectory() && IsFileValid(file))
            {
                ESP_LOGI(TAG, "Folder %02d has file %s", folderNumber, file.name());            
                if(lcd)
                {
                    char loadedText[10];
                    sprintf(loadedText, "Folder %02d", folderNumber);
                    lcd->Write(NULL, loadedText);
                }
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

    PlayerID3::ID3Tag& FolderManager::GetFile(uint8_t index)
    {
        PlayerID3::ID3Tag* tag = this->base;
        while(index > 0)
        {
            tag = tag->next;
            index--;
        }
        return *tag;
    }

    void FolderManager::Print()
    {
        PlayerID3::ID3Tag* tag = this->base;
        char* name;
        uint8_t i = 1;
        while(tag)
        {
            printf("File %d: ", i);
            if((name = tag->GetFrameValue("TIT2"))) printf("%s\n", name);
            else puts("");
            tag = tag->next;
            i++;
        }
    }

    FolderManager::~FolderManager()
    {
        ESP_LOGI(TAG, "Deleting folder %02d", number);
        PlayerID3::ID3Tag* currentFile = this->base;
        PlayerID3::ID3Tag* nextFile = NULL;
        while(currentFile)
        {
            nextFile = currentFile->next;
            delete currentFile;
            currentFile = nextFile;
        }
    }

    FileSystemManager::FileSystemManager(PlayerLCD::LCD* lcd)
    {
        root = "/";
        selected_folder = 0;
        char path[4] = "";
        for(uint8_t i = 0; i < 99; i++)
        {
            ESP_LOGI(TAG, "Loading folder %02d", i + 1);
            sprintf(path, "/%02d", i + 1);
            this->folders[i].Load(i + 1, lcd);
        }
    }

    FolderManager& FileSystemManager::GetFolder(uint8_t index)
    {
        return this->folders[index];
    }

    void FileSystemManager::Print()
    {
        puts("File System Manager Count");
        for(uint8_t i = 0; i < 99; i++)
        {
            if(this->folders[i].GetFileCount() > 0)
            {
                printf("Folder %02d: %d files.\n", i + 1, this->folders[i].GetFileCount());
            }
        }
    }
}