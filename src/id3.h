#ifndef ID3_H
#define ID3_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef enum Id3Flag
{
    ID3_FLAG_UNSYNCHRONISATION = 128,
    ID3_FLAG_EXTENDED_HEADER = 64,
    ID3_FLAG_EXPERIMENTAL_INDICATOR = 32,
    ID3_FLAG_FOOTER_PRESENT = 16
}Id3Flag;

typedef enum Id3ExtendedFlag
{
    ID3_XFLAG_TAG_IS_AN_UPDATE = 64,
    ID3_XFLAG_CRC_DATA_PRESENT = 32,
    ID3_XFLAG_TAG_RESTRICTIONS = 16
}Id3ExtendedFlag;

typedef enum Id3TagSizeRestrictions
{
    ID3_TAG_128_FRAMES_1_MB_MAX_SIZE = 0,
    ID3_TAG_64_FRAMES_128_KB_MAX_SIZE = 64,
    ID3_TAG_32_FRAMES_40_KB_MAX_SIZE = 128,
    ID3_TAG_32_FRAMES_4_KB_MAX_SIZE = 192
}Id3TagSizeRestrictions;

typedef enum Id3EncodingRestrictions
{
    ID3_ENCODING_ANY = 0,
    ID3_ENCODING_SPECIFIC = 32,
}Id3EncodingRestrictions;

typedef enum Id3StringRestrictions
{
    ID3_STRING_MAX_INF = 0,
    ID3_STRING_MAX_1024 = 8,
    ID3_STRING_MAX_128 = 16,
    ID3_STRING_MAX_30 = 24
}Id3StringRestrictions;

typedef enum Id3ImageRestrictions
{
    ID3_IMAGE_ENCODING_ANY = 0,
    ID3_IMAGE_ENCODING_SPECIFIC = 4,
    ID3_IMAGE_SIZE_MAX_INF = 0,
    ID3_IMAGE_SIZE_MAX_256 = 1,
    ID3_IMAGE_SIZE_MAX_64 = 2,
    ID3_IMAGE_SIZE_EXACTLY_64 = 3
}Id3ImageRestrictions;

typedef struct Id3Tag
{
    int16_t version;
    int8_t flags;
    int32_t headerSize;
    int32_t extendedHeaderSize;
    int8_t numberOfFlagBytes;
    int8_t extendedFlags;
    int64_t totalFrameCrc;
    int8_t restrictions;
}Id3Tag;

// Returns true if `mp3File` contains an ID3 tag.
bool IsId3TagPresent(FILE *mp3File);

// Returns the big-endian encoding of a little-endian value, and vice versa.
int64_t SwitchEndianness(int64_t value);

// Decodes the value represented by a synchsafe-encoded variable.
int64_t DecodeSynchsafe(int64_t value);

// Passes the ID3 tag information from `mp3File` to an `Id3Tag` struct.
// Returns `true` if successful, or `false` if the file does not contain an ID3 tag.
bool LoadId3Tag(FILE *mp3File, Id3Tag *tag);

// Prints the contents of an ID3 tag.
void PrintId3Tag(Id3Tag tag);

#endif