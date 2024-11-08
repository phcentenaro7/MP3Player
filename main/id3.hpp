#ifndef ID3_HPP
#define ID3_HPP
#include <SD.h>
#include <FS.h>
#include "esp_log.h"
#include "misc.hpp"

namespace PlayerID3
{
    bool FileHasID3Tag(File file);
    class ID3Header
    {
        private:
            uint8_t major_version;
            uint8_t revision_number;
            uint8_t flags;
            uint32_t tag_size;
        public:
            ID3Header(File file);
            inline uint8_t getMajorVersion() {return major_version;};
            inline uint8_t getRevisionNumber() {return revision_number;};
            inline uint8_t getFlags() {return flags;};
            inline uint32_t getTagSize() {return tag_size;};
    };
    const uint8_t FLAG_UNSYNCHRONIZATION = 0x80;
    const uint8_t FLAG_EXTENDED_HEADER = 0x40;
    const uint8_t FLAG_EXPERIMENTAL_INDICATOR = 0x20;
    const uint8_t FLAG_FOOTER_PRESENT = 0x10;
    class ID3ExtendedHeader
    {
        protected:
            uint32_t size;
            uint8_t flags;
            uint64_t CRC;
            uint8_t restrictions;
        public:
            ID3ExtendedHeader(File file);
            inline uint32_t getExtendedHeaderSize() {return size;};
            inline uint8_t getFlags() {return flags;};
            inline uint64_t getCRC() {return CRC;};
            inline uint8_t getRestrictions() {return restrictions;};
    };
    const uint8_t FLAG_TAG_IS_UPDATE = 0x40;
    const uint8_t FLAG_CRC_PRESENT = 0x20;
    const uint8_t FLAG_TAG_RESTRICTIONS = 0x10;
}

#endif