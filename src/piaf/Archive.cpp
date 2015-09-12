#include "Archive.h"
#include <cstring>
#include <cstdio>
#include <zlib.h>

using WalrusRPG::PIAF::Archive;
using WalrusRPG::PIAF::File;
using WalrusRPG::PIAF::FileEntry;
using WalrusRPG::PIAF::FileType;
using WalrusRPG::PIAF::CompressionType;

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

    // Must get a pointer on the file table.
    void load_file_table(FileEntry *entries, char *data, uint32_t nb_files)
    {
        for (unsigned index = 0; index < nb_files; index++)
        {
            char *current_entry_data = &data[index * 24];
            memcpy(entries[index].filename, current_entry_data, 8);
            entries[index].filename[8] = '\0'; // Makin' sure.
            entries[index].file_type =
                (FileType) read_big_endian_value<uint32_t>(&current_entry_data[8]);
            entries[index].compression_type =
                (CompressionType) read_big_endian_value<uint32_t>(
                    &current_entry_data[12]);
            entries[index].file_size =
                read_big_endian_value<uint32_t>(&current_entry_data[16]);
            entries[index].data_offset =
                read_big_endian_value<uint32_t>(&current_entry_data[20]);
        }
    }
}

Archive::Archive(std::string &filepath) : Archive(filepath.c_str())
{
}

Archive::Archive(const char *filepath) : file(nullptr), entries(nullptr)
{
    if (filepath == nullptr)
    {
        // TODO : throw NPE
        //fprintf(stderr, "Null filepath\n");
    }
    file = fopen(filepath, "rb");
    if (file == nullptr)
    {
        // TODO : throw Couldn't open
        //fprintf(stderr, "Unable to open %s\n", filepath);
    }
    // loading stuff happens NOW
    // checking if the file is long enough to have a header
    fseek(file, 0L, SEEK_END);
    uint64_t filesize = ftell(file);
    fseek(file, 0L, SEEK_SET);
    if (filesize < 32)
    {
        // TODO : throw file too small
        //fprintf(stderr, "File too small\n");
    }

    char header_container[32] = {0};
    fread(header_container, sizeof(char), 32, file);
    if (strncmp(header_container, "WRPGPIAF", 8) != 0)
    {
        // TODO throw bad header
        //fprintf(stderr, "Bad header magic word\n");
    }
    uint32_t expected_checksum = read_big_endian_value<uint32_t>(&header_container[8]);
    uint32_t calculated_checksum = crc32(0L, (unsigned char *) &header_container[16], 16);
    if (expected_checksum != calculated_checksum)
    {
        // TODO throw bad checksum
        //fprintf(stderr, "Bad header checksum : %x != %x\n", expected_checksum, calculated_checksum);
    }

    // TODO : version checking
    version = read_big_endian_value<uint32_t>(&header_container[16]);
    /* if (version != CURRENT_PIAF_VERSION)
     * {
     *      exception up;
     *      throw up; // haha
     * }
     */
    nb_files = read_big_endian_value<uint32_t>(&header_container[20]);
    // printf("nb_files : %u\n", nb_files);

    data_size = read_big_endian_value<uint32_t>(&header_container[24]);
    uint64_t calculated_data_size = filesize - 32 - 24 * nb_files;
    if (data_size != calculated_data_size)
    {
        // T0D0 : throw wrong size exception
        // fprintf(stderr, "Bad data size : expected %u, got %lld\n", data_size, calculated_data_size);
    }
    if (nb_files != 0)
    {
        uint32_t expected_filetable_checksum =
            read_big_endian_value<uint32_t>(&header_container[12]);
        char *file_entry_data = new char[24 * nb_files];
        fseek(file, 32, SEEK_SET);
        fread(file_entry_data, sizeof(char), 24 * nb_files, file);
        if (expected_filetable_checksum !=
            crc32(0L, (unsigned char *) file_entry_data, 24 * nb_files))
        {
            // TODO : checksum exception
            // fprintf(stderr, "Bad filetable checksum\n");
        }
        entries = new FileEntry[nb_files];
        load_file_table(entries, file_entry_data, nb_files);
        delete[] file_entry_data;
    }
}

Archive::~Archive()
{
    if(file != nullptr)
        fclose(file);
    if( entries != nullptr)
        delete[] entries;
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

File Archive::get(char *filename)
{
    for (unsigned index = 0; index < nb_files; index++)
    {
        if (strncmp(filename, entries[index].filename, 8) == 0)
        {
            uint8_t *data = new uint8_t[entries[index].file_size];
            fseek(file, entries[index].data_offset + 32 + 24 * nb_files, SEEK_SET);
            if (fread(data, sizeof(uint8_t), entries[index].file_size, file) != entries[index].file_size)
            {
                // throw loading error
            }
            return File(data, entries[index].file_size);

        }
    }
    // throw not found exception
}


File::File(uint8_t *data, std::size_t size) : data(data), size(size)
{
}

File::~File()
{
    delete[] data;
}

uint8_t& File::operator[]( std::size_t idx)
{
    if (idx >= size)
    {
        // throw up
    }
    return data[idx];
}
