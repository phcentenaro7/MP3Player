#include "misc.hpp"

namespace PlayerMisc
{
    bool StringEndsWith(const char* string, const char* end)
    {
        size_t string_index = strlen(string);
        size_t end_index = strlen(end);
        if(end_index > string_index)
        {
            return false;
        }
        while(end_index > 0)
        {
            if(string[--string_index] != end[--end_index])
            {
                return false;
            }
        }
        return true;
    }
    void SwitchEndianness(uint32_t& bytes)
    {
        bytes =   (bytes >> 24)
                | ((bytes & 0xff0000) >> 8)
                | ((bytes & 0xff00) << 8)
                | (bytes << 24);
    }
    void SwitchEndianness(uint64_t& bytes)
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
    bool IsSynchsafe(uint32_t bytes)
    {
        return (bytes & 0x80808080) == 0;
    }
    bool IsSynchsafe(uint64_t bytes)
    {
        return (bytes & 0x8080808080808080) == 0;
    }
    void UndoSynchsafe(uint32_t& bytes)
    {
        bytes =   (bytes & 0xff)
                | ((bytes & 0xff00) >> 1)
                | ((bytes & 0xff0000) >> 2)
                | ((bytes & 0xff000000) >> 3);
    }
    void UndoSynchsafe(uint64_t& bytes)
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
}
