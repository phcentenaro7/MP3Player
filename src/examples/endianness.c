#include "../id3.h"
#include <stdio.h>
#include <stdint.h>

int main()
{
    int64_t var = 0x123456789ABCDEFF;
    printf("Original: %llx\n", var);
    printf("Switched: %llx\n", SwitchEndianness(var));
    return 0;
}