#ifndef ID3_H
#define ID3_H
#include <stdbool.h>
#include <stdio.h>
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

typedef struct Id3Tag
{
    int16_t version;
    int8_t flags;
    int32_t headerSize;
    int32_t extendedHeaderSize;
    int8_t numberOfFlagBytes;
    int8_t extendedFlags;
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