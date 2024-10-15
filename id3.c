#include "id3.h"
#include <stdio.h>

bool IsId3TagPresent(FILE *mp3File)
{
    fseek(mp3File, 0, SEEK_SET);
    char fileStart[3];
    fread(fileStart, sizeof(char), 3, mp3File);
    return (fileStart[0] == 'I' && fileStart[1] == 'D' && fileStart[2] == '3');
}

void SwitchEndianness(int64_t *value)
{
    *value = 0 | ((*value >> 56) & (int64_t)0Xff)
               | ((*value >> 40) & (int64_t)0xff00)
               | ((*value >> 24) & (int64_t)0xff0000)
               | ((*value >> 8)  & (int64_t)0xff000000)
               | ((*value & (int64_t)0xff000000) << 8)
               | ((*value & (int64_t)0xff0000) << 24)
               | ((*value & (int64_t)0xff00) << 40)
               | ((*value & (int64_t)0xff) << 56);
}

void DecodeSynchsafe(int64_t *value)
{
    *value = 0 | (*value & 0xff)
               | ((*value & (int64_t)0xff00) >> 1)
               | ((*value & (int64_t)0xff0000) >> 2)
               | ((*value & (int64_t)0xff000000) >> 3)
               | ((*value & 0xff00000000) >> 4)
               | ((*value & 0xff0000000000) >> 5)
               | ((*value & 0xff000000000000) >> 6)
               | ((*value & 0xff00000000000000) >> 7);
}

void SwitchEndiannessAndDecodeSynchsafe(int64_t *value)
{
    SwitchEndianness(value);
    DecodeSynchsafe(value);
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
    SwitchEndiannessAndDecodeSynchsafe(tag->headerSize);
    if(tag->flags & ID3_FLAG_EXTENDED_HEADER)
    {
        fread(&(tag->extendedHeaderSize), sizeof(int32_t), 1, mp3File);
        SwitchEndiannessAndDecodeSynchsafe(tag->extendedHeaderSize);
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