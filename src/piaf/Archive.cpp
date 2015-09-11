#include "Archive.h"
#include <cstring>
#include <cstdio>
#include <zlib.h>

using WalrusRPG::PIAF::Archive;
using WalrusRPG::PIAF::FileEntry;
using WalrusRPG::PIAF::FileType;

namespace
{
    template <typename T> T read_big_endian_value(void *ptr)
    {
        T result = 0;
        uint8_t *data = (uint8_t *) ptr;
        for (unsigned i = 0; i < sizeof(T); i++)
            result |= data[i] << (8 * (sizeof(T) - 1 - i));
        return result;
    }
}

Archive::Archive(char *filepath)
{
    if (filepath == nullptr)
    {
    }
    // TODO : throw NPE
    FILE *file = fopen(filepath, "rb");
    if (file == nullptr)
    {
        // TODO : throw Couldn't open
    }
    // loading stuff happens NOW
    // checking if the file is long enough to have a header
    fseek(file, 0L, SEEK_END);
    uint64_t filesize = ftell(file);
    fseek(file, 0L, SEEK_SET);
    if (filesize < 32)
    {
        // TODO : throw file too small
    }

    char header_container[32] = {0};
    fread(header_container, sizeof(char), 32, file);
    if (strncmp(header_container, "WRPGPIAF", 8) != 0)
    {
        // TODO throw bad header
    }
    uint32_t expected_checksum = read_big_endian_value<uint32_t>(&header_container[8]);
    if (expected_checksum != crc32(0L, (unsigned char*)header_container, 32))
    {
        // TODO throw bad checksum
    }

    // TODO : version checking
    version = read_big_endian_value<uint32_t>(&header_container[12]);



    // ...
    // entries = new File_Entry[];
    // ...
}

Archive::~Archive()
{
    // delete[] entries;
}

FileEntry Archive::get_file_entry(char *filename)
{
    for (uint32_t index = 0; index < nb_files; index++)
    {
        if (strcmp(entries[index].filename, filename) == 0)
        {
            return entries[index];
        }
    }
    // throw exception
}

/*PFile*/ void /*FileEntry*/ get(char *filename)
{
    // return PFile(get_file_entry(filename));
}
