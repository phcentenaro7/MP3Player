#ifndef MISC_HPP
#define MISC_HPP
#include <FS.h>
#include <string.h>

namespace PlayerMisc
{
    bool StringEndsWith(const char* string, const char* end);

    void TrimLeftNUL(uint8_t* string, size_t stringLength);

    inline void SwitchEndianness(uint16_t& bytes)
    {
        bytes = (bytes >> 8) | (bytes << 8);
    }

    inline void SwitchEndianness(uint32_t& bytes)
    {
        bytes =   (bytes >> 24)
                | ((bytes & 0xff0000) >> 8)
                | ((bytes & 0xff00) << 8)
                | (bytes << 24);
    }

    inline void SwitchEndianness(uint64_t& bytes)
    {
        bytes =   (bytes >> 56)
                | (bytes >> 40 & 0xff00)
                | (bytes >> 24 & 0xff0000)
                | (bytes >> 8 & 0xff000000)
                | ((bytes & 0xff000000) << 8)
                | ((bytes & 0xff0000) << 24)
                | ((bytes & 0xff00) << 40)
                | ((bytes & 0xff) << 56);
    }

    inline bool IsSynchsafe(uint16_t bytes)
    {
        return (bytes & 0x8080) == 0;
    }

    inline bool IsSynchsafe(uint32_t bytes)
    {
        return (bytes & 0x80808080) == 0;
    }

    inline bool IsSynchsafe(uint64_t bytes)
    {
        return (bytes & 0x8080808080808080) == 0;
    }

    inline void UndoSynchsafe(uint16_t& bytes)
    {
        bytes =   (bytes & 0xff)
                | ((bytes & 0xff00) >> 1);
    }

    inline void UndoSynchsafe(uint32_t& bytes)
    {
        bytes =   (bytes & 0xff)
                | ((bytes & 0xff00) >> 1)
                | ((bytes & 0xff0000) >> 2)
                | ((bytes & 0xff000000) >> 3);
    }

    inline void UndoSynchsafe(uint64_t& bytes)
    {
        bytes =   (bytes & 0xff)
                | ((bytes & 0xff00) >> 1)
                | ((bytes & 0xff0000) >> 2)
                | ((bytes & 0xff000000) >> 3)
                | ((bytes & 0xff00000000) >> 4)
                | ((bytes & 0xff0000000000) >> 5)
                | ((bytes & 0xff000000000000) >> 6)
                | ((bytes & 0xff00000000000000) >> 7);
    }

    inline void SwitchEndiannessAndUndoSynchsafe(uint16_t& bytes)
    {
        SwitchEndianness(bytes);
        UndoSynchsafe(bytes);
    }

    inline void SwitchEndiannessAndUndoSynchsafe(uint32_t& bytes)
    {
        SwitchEndianness(bytes);
        UndoSynchsafe(bytes);
    }

    inline void SwitchEndiannessAndUndoSynchsafe(uint64_t& bytes)
    {
        SwitchEndianness(bytes);
        UndoSynchsafe(bytes);
    }
}

#endif