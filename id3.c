#include "id3.h"
#include <stdio.h>

bool IsId3TagPresent(FILE *mp3File)
{
    fseek(mp3File, 0, SEEK_SET);
    char fileStart[3];
    fread(fileStart, sizeof(char), 3, mp3File);
    return (fileStart[0] == 'I' && fileStart[1] == 'D' && fileStart[2] == '3');
}

void ConvertHeaderSizeToBigEndian(int32_t *headerSize)
{
    *headerSize = 0 | ((*headerSize & 0x7F000000) >> 24)
                    | ((*headerSize & 0x7F0000) >> 9)
                    | ((*headerSize & 0x7F00) << 6)
                    | ((*headerSize & 0x7F) << 21);
    printf("%x\n", *headerSize);
}

bool LoadId3Tag(FILE *mp3File, Id3Tag *tag)
{
    if(!IsId3TagPresent(mp3File))
    {
        return false;
    }
    fread(&(tag->version), sizeof(int16_t), 1, mp3File);
    fread(&(tag->flags), sizeof(int8_t), 1, mp3File);
    fread(&(tag->headerSize), sizeof(int32_t), 1, mp3File);
    ConvertHeaderSizeToBigEndian(&(tag->headerSize));
    if(tag->flags & ID3_FLAG_EXTENDED_HEADER)
    {
        fread(&(tag->extendedHeaderSize), sizeof(int32_t), 1, mp3File);
        ConvertHeaderSizeToBigEndian(&(tag->extendedHeaderSize));
        fread(&(tag->numberOfFlagBytes), sizeof(int8_t), 1, mp3File);
        fread(&(tag->extendedFlags), sizeof(int8_t), 1, mp3File);
    }
    else
    {
        tag->extendedHeaderSize = 0;
        tag->numberOfFlagBytes = 0;
        tag->extendedFlags = 0;
    }
}

void PrintId3Tag(Id3Tag tag)
{
    puts("***** ID3 TAG INFORMATION *****");
    puts("");
    puts("***** GENERAL INFORMATION *****");
    printf("Version:\t\t %d\n", tag.version);
    printf("Tag Size:\t\t %d\n", tag.headerSize);
    printf("Extended Header Size:\t %d\n", tag.extendedHeaderSize);
    puts("");
    puts("***** HEADER FLAGS *****");
    printf("Unsynchronisation:\t %d\n", tag.flags & ID3_FLAG_UNSYNCHRONISATION);
    printf("Extended Header:\t %d\n", tag.flags & ID3_FLAG_EXTENDED_HEADER);
    printf("Experimental Indicator:\t %d\n", tag.flags & ID3_FLAG_EXPERIMENTAL_INDICATOR);
    printf("Footer Present:\t\t %d\n", tag.flags & ID3_FLAG_FOOTER_PRESENT);
    puts("");
    puts("***** EXTENDED FLAGS *****");
    printf("Tag is an Update:\t %d\n", tag.extendedFlags & ID3_XFLAG_TAG_IS_AN_UPDATE);
    printf("CRC Data Present:\t %d\n", tag.extendedFlags & ID3_XFLAG_CRC_DATA_PRESENT);
    printf("Tag Restrictions:\t %d\n", tag.extendedFlags & ID3_XFLAG_TAG_RESTRICTIONS);
}