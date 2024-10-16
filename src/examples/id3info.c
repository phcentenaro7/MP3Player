#include "../id3.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *mp3File = fopen(argv[1], "r");
    Id3Tag tag;
    LoadId3Tag(mp3File, &tag);
    PrintId3Tag(tag);
    return 0;
}