#include "id3.hpp"

namespace PlayerID3
{
    bool FileHasID3Tag(File file)
    {
        char SOF[3];
        file.readBytes(SOF, 3 * sizeof(char));
        return (SOF[0] == 'I' && SOF[1] == 'D' && SOF[2] == '3');
    }
    ID3Header::ID3Header(File file)
    {
        file.read(&(this->major_version), sizeof(uint8_t));
        file.read(&(this->revision_number), sizeof(uint8_t));
        file.read(&(this->flags), sizeof(uint8_t));
        file.read((uint8_t*)&(this->size), sizeof(uint32_t));
    }
}