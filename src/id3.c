#include "id3.h"

bool IsId3TagPresent(FILE *mp3File)
{
    fseek(mp3File, 0, SEEK_SET);
    char fileStart[3];
    fread(fileStart, sizeof(char), 3, mp3File);
    return (fileStart[0] == 'I' && fileStart[1] == 'D' && fileStart[2] == '3');
}

int64_t SwitchEndianness(int64_t value)
{
    return 0 | ((value >> 56) & (int64_t)0Xff)
             | ((value >> 40) & (int64_t)0xff00)
             | ((value >> 24) & (int64_t)0xff0000)
             | ((value >> 8)  & (int64_t)0xff000000)
             | ((value & (int64_t)0xff000000) << 8)
             | ((value & (int64_t)0xff0000) << 24)
             | ((value & (int64_t)0xff00) << 40)
             | ((value & (int64_t)0xff) << 56);
}

int64_t DecodeSynchsafe(int64_t value)
{
    return 0 | (value & 0xff)
             | ((value & (int64_t)0xff00) >> 1)
             | ((value & (int64_t)0xff0000) >> 2)
             | ((value & (int64_t)0xff000000) >> 3)
             | ((value & 0xff00000000) >> 4)
             | ((value & 0xff0000000000) >> 5)
             | ((value & 0xff000000000000) >> 6)
             | ((value & 0xff00000000000000) >> 7);
}

void LoadId3Header(FILE *mp3File, Id3Tag *tag)
{
    fread(&(tag->version), sizeof(int16_t), 1, mp3File);
    fread(&(tag->flags), sizeof(int8_t), 1, mp3File);
    fread(&(tag->size), sizeof(int32_t), 1, mp3File);
    tag->size = (int32_t)DecodeSynchsafe(SwitchEndianness(tag->size) >> 32);
}

void LoadId3ExtendedHeader(FILE *mp3File, Id3Tag *tag)
{
    if(!(tag->flags & ID3_FLAG_EXTENDED_HEADER))
    {
        return;
    }
    else
    {
        fread(&(tag->extendedHeaderSize), sizeof(int32_t), 1, mp3File);
        tag->extendedHeaderSize = (int32_t)DecodeSynchsafe(SwitchEndianness(tag->extendedHeaderSize) >> 32);
        fread(&(tag->numberOfFlagBytes), sizeof(int8_t), 1, mp3File);
        fread(&(tag->extendedFlags), sizeof(int8_t), 1, mp3File);
    }
    if(tag->extendedFlags & ID3_XFLAG_TAG_IS_AN_UPDATE)
    {
        fread(NULL, sizeof(int8_t), 1, mp3File);
    }
    if(tag->extendedFlags & ID3_XFLAG_CRC_DATA_PRESENT)
    {
        fread(NULL, sizeof(int8_t), 1, mp3File);
        fread(&(tag->totalFrameCrc), sizeof(int64_t), 1, mp3File);
        tag->totalFrameCrc = DecodeSynchsafe(SwitchEndianness(tag->totalFrameCrc) >> 24);
    }
    if(tag->extendedFlags & ID3_XFLAG_TAG_RESTRICTIONS)
    {
        fread(NULL, sizeof(int8_t), 1, mp3File);
        fread(&(tag->restrictions), sizeof(int8_t), 1, mp3File);
    }
}

bool LoadId3Tag(FILE *mp3File, Id3Tag *tag)
{
    memset(tag, 0, sizeof(Id3Tag));
    if(!IsId3TagPresent(mp3File))
    {
        return false;
    }
    LoadId3Header(mp3File, tag);
    LoadId3ExtendedHeader(mp3File, tag);
    return true;
}

void PrintId3Tag(Id3Tag tag)
{
    puts("***** ID3 TAG INFORMATION *****");
    puts("");
    puts("***** GENERAL INFORMATION *****");
    printf("Version:\t\t %d\n", tag.version);
    printf("Tag Size:\t\t %d\n", tag.size);
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