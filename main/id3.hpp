#ifndef ID3_HPP
#define ID3_HPP
#include <stdio.h>
#include <string.h>
#include <SD.h>
#include <FS.h>
#include "esp_log.h"
#include "misc.hpp"

namespace PlayerID3
{
    const uint8_t FLAG_UNSYNCHRONIZATION = 0x80;
    const uint8_t FLAG_EXTENDED_HEADER = 0x40;
    const uint8_t FLAG_EXPERIMENTAL_INDICATOR = 0x20;
    const uint8_t FLAG_FOOTER_PRESENT = 0x10;
    const uint8_t FLAG_TAG_IS_UPDATE = 0x40;
    const uint8_t FLAG_CRC_PRESENT = 0x20;
    const uint8_t FLAG_TAG_RESTRICTIONS = 0x10;

    bool FileHasID3Tag(File file);

    class ID3Header
    {
        private:
            uint8_t major_version;
            uint8_t revision_number;
            uint8_t flags;
            uint32_t tag_size;
        public:
            ID3Header() : major_version(0), revision_number(0), flags(0), tag_size(0){};
            void Load(File file);
            inline uint8_t GetMajorVersion() {return major_version;};
            inline uint8_t GetRevisionNumber() {return revision_number;};
            inline uint8_t GetFlags() {return flags;};
            inline uint32_t GetTagSize() {return tag_size;};
            void Print();
    };

    class ID3ExtendedHeader
    {
        private:
            uint32_t size;
            uint8_t flags;
            uint64_t CRC;
            uint8_t restrictions;
        public:
            ID3ExtendedHeader() : size(0), flags(0), CRC(0), restrictions(0){};
            void Load(File file);
            inline uint32_t GetExtendedHeaderSize() {return size;};
            inline uint8_t GetFlags() {return flags;};
            inline uint64_t GetCRC() {return CRC;};
            inline uint8_t GetRestrictions() {return restrictions;};
            void Print();
    };

    struct ID3Frame
    {
        char* key;
        char* value;
        ID3Frame* next;
        ID3Frame(char* key, char* value);
        void Print();
        ~ID3Frame();
    };

    class ID3FrameList
    {
        private:
            ID3Frame* base;
            size_t count;
        public:
            ID3FrameList() : base(NULL), count(0){};
            inline size_t GetFrameCount() {return count;};
            void AddFrame(char* key, char* value);
            bool DoesFrameExist(char* key);
            char* GetFrameValue(char* key);
            void Print();
            ~ID3FrameList();
    };

    class ID3Tag : public ID3Header, public ID3ExtendedHeader, public ID3FrameList
    {
        public:
            ID3Tag(File file);
            ID3Tag* next;
            void Print();
    };
}

#endif