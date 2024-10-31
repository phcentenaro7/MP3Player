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
}
