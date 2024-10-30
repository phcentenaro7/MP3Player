#ifndef MISC_HPP
#define MISC_HPP
#include <FS.h>
#include <string.h>

namespace PlayerMisc
{
    bool StringEndsWith(const char* string, const char* end);
    void SwitchEndianness(uint32_t& bytes);
    void SwitchEndianness(uint64_t& bytes);
    bool IsSynchsafe(uint32_t bytes);
    bool IsSynchsafe(uint64_t bytes);
    void UndoSynchsafe(uint32_t& bytes);
    void UndoSynchsafe(uint64_t& bytes);
}

#endif