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

    void TrimLeftNUL(char* string, size_t stringLength)
    {
        size_t shiftLength = 0;
        for(size_t i = 0; string[i] == '\0' && i < stringLength; i++, shiftLength++);
        if(shiftLength > 0)
        {
            memmove(string, string + shiftLength, stringLength - shiftLength);
            string[stringLength - shiftLength] = '\0';
        }
    }
}
