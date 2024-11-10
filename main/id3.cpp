#include "id3.hpp"

static const char* TAG = "ID3";

namespace PlayerID3
{
    bool FileHasID3Tag(File file)
    {
        uint8_t SOF[4] = {0};
        file.read(SOF, 3 * sizeof(int8_t));
        return !strcmp((const char*)SOF, "ID3");
    }

    void ID3Header::Load(File file)
    {
        file.read(&(this->major_version), sizeof(uint8_t));
        file.read(&(this->revision_number), sizeof(uint8_t));
        file.read(&(this->flags), sizeof(uint8_t));
        file.read((uint8_t*)&(this->tag_size), sizeof(uint32_t));
        PlayerMisc::SwitchEndiannessAndUndoSynchsafe(this->tag_size);
    }

    void ID3Header::Print()
    {
        printf("Version: %d.%d\n", this->major_version, this->revision_number);
        printf("Tag size: %lu\n", this->tag_size);
        if(this->flags == 0)
        {
            printf("Header has no flags.\n");
            return;
        }
        printf("Header has the following flags:\n");
        if(this->flags & FLAG_UNSYNCHRONIZATION) printf("\t* Unsynchronization\n");
        if(this->flags & FLAG_EXTENDED_HEADER) printf("\t* Extended header\n");
        if(this->flags & FLAG_EXPERIMENTAL_INDICATOR) printf("\t* Experimental indicator\n");
        if(this->flags & FLAG_FOOTER_PRESENT) printf("\t* Footer present\n");
    }

    void ID3ExtendedHeader::Load(File file)
    {
        file.read((uint8_t*)&(this->size), sizeof(uint32_t));
        PlayerMisc::SwitchEndiannessAndUndoSynchsafe(this->size);
        file.seek(sizeof(uint8_t), fs::SeekCur);
        file.read(&(this->flags), sizeof(uint8_t));
        size_t seek_offset = 0;
        if(this->flags & FLAG_TAG_IS_UPDATE)
        {
            file.seek(sizeof(uint8_t), fs::SeekCur);
            seek_offset++;
        }
        if(this->flags & FLAG_CRC_PRESENT)
        {
            file.read((uint8_t*)&(this->CRC), sizeof(uint64_t));
            PlayerMisc::SwitchEndiannessAndUndoSynchsafe(this->CRC);
            seek_offset += sizeof(uint64_t);
        }
        if(this->flags & FLAG_TAG_RESTRICTIONS)
        {
            file.seek(sizeof(uint8_t), fs::SeekCur);
            file.read(&(this->restrictions), sizeof(uint8_t));
            seek_offset += sizeof(uint8_t) + sizeof(uint8_t);
        }
        file.seek(this->size - seek_offset - 6, fs::SeekCur);
    }

    void ID3ExtendedHeader::Print()
    {
        printf("Extended header size: %lu\n", this->size);
        if(this->flags == 0)
        {
            printf("Extended header has no flags.\n");
        }
        if(this->flags & FLAG_TAG_IS_UPDATE) printf("Tag has been marked as an update.\n");
        if(this->flags & FLAG_CRC_PRESENT) printf("Tag contains CRC %llu.\n", this->CRC);
        if(this->flags & FLAG_TAG_RESTRICTIONS) printf("Tag contains additional restrictions (sum %x).\n", this->restrictions);
    }

    ID3Frame::ID3Frame(uint8_t* key, uint8_t* value)
    {
        this->key = key;
        this->value = value;
        this->next = NULL;
    }

    void ID3Frame::Print()
    {
        printf("Frame key: %s\n", this->key);
        printf("Frame value: %s\n", this->value);
    }

    ID3Frame::~ID3Frame()
    {
        delete this->key;
        delete this->value;
    }

    void ID3FrameList::AddFrame(uint8_t* key, uint8_t* value)
    {
        size_t framesRemaining = this->count;
        if(framesRemaining == 0)
        {
            this->base = new ID3Frame(key, value);
            count++;
            return;
        }
        ID3Frame* currentFrame = this->base;
        while(currentFrame->next != NULL)
        {
            currentFrame = currentFrame->next;
        }
        currentFrame->next = new ID3Frame(key, value);
        count++;
    }

    bool ID3FrameList::DoesFrameExist(uint8_t* key)
    {
        ID3Frame* currentFrame = this->base;
        while(currentFrame != NULL)
        {
            if(!strcmp((const char*)currentFrame->key, (const char*)key)) return true;
            currentFrame = currentFrame->next;
        }
        return false;
    }

    uint8_t* ID3FrameList::GetFrameValue(uint8_t* key)
    {
        ID3Frame* currentFrame = this->base;
        while(currentFrame != NULL)
        {
            if(!strcmp((const char*)currentFrame->key, (const char*)key)) return currentFrame->value;
            currentFrame = currentFrame->next;
        }
        return NULL;
    }

    void ID3FrameList::Print()
    {
        ID3Frame* currentFrame = this->base;
        while(currentFrame != NULL)
        {
            currentFrame->Print();
            currentFrame = currentFrame->next;
        }
    }

    ID3Tag::ID3Tag(File file)
    {
        ESP_LOGI(TAG, "Loading ID3 tag from file %s", file.name());
        if(!FileHasID3Tag(file))
        {
            ESP_LOGE(TAG, "File %s does not have an ID3 header", file.name());
        }
        this->ID3Header::Load(file);
        if(this->ID3Header::GetFlags() & FLAG_EXTENDED_HEADER)
        {
            this->ID3ExtendedHeader::Load(file);
        }
        size_t remainingTagLength = this->GetTagSize() - this->GetExtendedHeaderSize();
        while(remainingTagLength >= 10)
        {
            uint8_t* key = new uint8_t[5]();
            file.read(key, 4 * sizeof(uint8_t));
            if(!strcmp((const char*)key, "")) break;
            uint32_t value_size;
            file.read((uint8_t*)&(value_size), sizeof(uint32_t));
            PlayerMisc::SwitchEndiannessAndUndoSynchsafe(value_size);
            file.seek(2 * sizeof(uint8_t), fs::SeekCur);
            uint8_t* value = new uint8_t[value_size];
            file.read(value, value_size * sizeof(uint8_t));
            PlayerMisc::TrimLeftNUL(value, value_size);
            this->AddFrame(key, value);
            remainingTagLength -= 14 + value_size;
        }
        return;
    }

    void ID3Tag::Print()
    {
        printf("Tag Information\n");
        this->ID3Header::Print();
        this->ID3ExtendedHeader::Print();
        this->ID3FrameList::Print();
        puts("");
    }
}