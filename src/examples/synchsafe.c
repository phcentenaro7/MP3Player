#include "../id3.h"
#include <stdio.h>

int main()
{
    int64_t var = 0x7f7f7f7f7f7f7f7f;
    printf("Synchsafe: %llx\n", var);
    printf("Decoded: %llx\n", DecodeSynchsafe(var));
    return 0;
}