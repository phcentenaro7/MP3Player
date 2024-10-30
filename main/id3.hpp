#ifndef ID3_HPP
#define ID3_HPP
#include <SD.h>
#include <FS.h>

namespace PlayerID3
{
    bool FileHasID3Tag(File file);
    class ID3Header
    {
        protected:
            
        public:
            uint8_t major_version;
            uint8_t revision_number;
            uint8_t flags;
            uint32_t size;
            ID3Header(File file);
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
    };
    const uint8_t FLAG_TAG_IS_UPDATE = 0x40;
    const uint8_t FLAG_CRC_PRESENT = 0x20;
    const uint8_t FLAG_TAG_RESTRICTIONS = 0x10;
}

#endif