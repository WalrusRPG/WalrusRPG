#include <cstring>
#include <cstdio>
#include <memory>
#include <zlib.h>
#include "Archive.h"
#include "Quirks.h"

using tinystl::string;
using WalrusRPG::PIAF::Archive;
using WalrusRPG::PIAF::File;
using WalrusRPG::PIAF::FileType;
using WalrusRPG::PIAF::CompressionType;

namespace
{
    /**
     * Read big endian integer value in binary data array. Depends on the size
     * of the given type, so using standardized types like stdint's is highly
     * suggested.
     *
     * Also doesn't check for being in bounds of the array.
     */
      template <typename T> T read_big_endian_value(void *ptr)
    {
        T result = 0;
        uint8_t *data = (uint8_t *) ptr;
        for (unsigned i = 0; i < sizeof(T); i++)
            // Writing each byte of value in order
            result |= data[i] << (8 * (sizeof(T) - 1 - i));
        return result;
    }

    // Must get a pointer on the file table.
    /**
     * Reads the file table from given data pointer to an empty FileEntry
     * array long enough and a given number of files to load.
     * The pointer must directly access the file entry region of the archive.
     */
    void load_file_table(File *entries, uint32_t *data_offsets, char *data, uint32_t nb_files)
    {
        for (unsigned index = 0; index < nb_files; index++)
        {
            // Offsetting the data pointer to the current file entry.
            char *current_entry_data = &data[index * 24];

            // Copying the filename to the FileEntry
            memcpy(entries[index].filename, current_entry_data, 8);
            // Making sure there is a terminating \0 character in the filename.
            entries[index].filename[8] = '\0';
            // /!\ Parsing a value to an enumeration.
            // Parses the file type and store it.
            entries[index].file_type =
                (FileType) read_big_endian_value<uint32_t>(&current_entry_data[8]);
            // /!\ Parsing a value to an enumeration.
            // Parses the compression type and store it.
            entries[index].compression_type =
                (CompressionType) read_big_endian_value<uint32_t>(
                    &current_entry_data[12]);
            // Parsign the file size and storing it.
            entries[index].file_size =
                read_big_endian_value<uint32_t>(&current_entry_data[16]);
            // Reading the file's data position in the archive data section.
            data_offsets[index] =
                read_big_endian_value<uint32_t>(&current_entry_data[20]);
        }
    }
}

Archive::Archive(string &filepath) : Archive(filepath.c_str())
{
}

Archive::Archive(const char *filepath) : file(nullptr), entries(nullptr), files_data(nullptr), files_loaded(nullptr)
{
    // Null pointer exception trigger
    if (filepath == nullptr)
    {
        // TODO : throw NPE
        // fprintf(stderr, "Null filepath\n");
    }
    // Solves the absolute path for given relative path.
    // Must be needed in targets like Ndless as it doesn't support environment
    // vars and thus PATH.
    std::unique_ptr<char> real_filename(Quirks::solve_absolute_path(filepath));

    // Open the archive
    file = fopen(real_filename.get(), "rb");
    // Again another null pointer trigger
    if (file == nullptr)
    {
        // TODO : throw Couldn't open
        // fprintf(stderr, "Unable to open %s\n", filepath);
    }

    // Loading stuff happens NOW
    // Checking if the file is long enough to have a header
    fseek(file, 0L, SEEK_END);
    uint64_t filesize = ftell(file);
    fseek(file, 0L, SEEK_SET);
    // File to small exception trigger
    if (filesize < 32)
    {
        // TODO : throw file too small
        // fprintf(stderr, "File too small\n");
    }

    // Tempoary buffer to contain the header.
    char header_container[32] = {0};
    // Read the headers and trigger exceptions on errors
    if (fread(header_container, sizeof(char), 32, file))
    {
        // So we coudln't load the whole header.
        // TODO : check flags and return correct exceptions
        // fprintf(stderr, "Error loading header\n");

    }
    // Check if the magic cookie is the same.
    // It's a first way to detect if the file is correctly an archive.
    if (strncmp(header_container, "WRPGPIAF", 8) != 0)
    {
        // TODO throw bad header
        // fprintf(stderr, "Bad header magic word\n");
    }
    // Checksum time! Let's check if the header hasn"t been altered.
    uint32_t expected_checksum = read_big_endian_value<uint32_t>(&header_container[8]);
    uint32_t calculated_checksum = crc32(0L, (unsigned char *) &header_container[16], 16);
    if (expected_checksum != calculated_checksum)
    {
        // TODO throw bad checksum
        // fprintf(stderr, "Bad header checksum : %x != %x\n", expected_checksum,
        // calculated_checksum);
    }

    // TODO : version checking
    version = read_big_endian_value<uint32_t>(&header_container[16]);
    /* if (version != CURRENT_PIAF_VERSION)
     * {
     *      exception up;
     *      throw up; // haha
     * }
     */

    // At this point, the archive header looks unaltered and we finally can parse
    // and load the header.

    // Read the archive's number of files
    nb_files = read_big_endian_value<uint32_t>(&header_container[20]);
    // printf("nb_files : %u\n", nb_files);

    data_size = read_big_endian_value<uint32_t>(&header_container[24]);
    uint64_t calculated_data_size = filesize - 32 - 24 * nb_files;
    if (data_size != calculated_data_size)
    {
        // T0D0 : throw wrong size exception
        // fprintf(stderr, "Bad data size : expected %u, got %lld\n", data_size,
        // calculated_data_size);
    }
    // Check if there are files to manage.
    if (nb_files != 0)
    {
        // So, the file table is not empty. let's check if it's
        // not altered.
        uint32_t expected_filetable_checksum =
            read_big_endian_value<uint32_t>(&header_container[12]);
        char *file_entry_data = new char[24 * nb_files];
        fseek(file, 32, SEEK_SET);
        fread(file_entry_data, sizeof(char), 24 * nb_files, file);
        // Compare and trigger an exception if the checksum doesn't match.
        if (expected_filetable_checksum !=
            crc32(0L, (unsigned char *) file_entry_data, 24 * nb_files))
        {
            // TODO : checksum exception
            // fprintf(stderr, "Bad filetable checksum\n");
        }
        // Create the filetable.
        entries = new File[nb_files];
        // Parse and story the filetable.

        files_data = new uint8_t*[nb_files];
        files_loaded = new bool[nb_files];
        files_data_offset = new uint32_t[nb_files];
        for(unsigned i = 0; i < nb_files; i++)
        {
            files_data[i] = nullptr;
            files_loaded[i] = false;
        }

        load_file_table(entries, files_data_offset, file_entry_data, nb_files);
        delete[] file_entry_data;


    }
}

Archive::~Archive()
{
    if (file != nullptr)
        fclose(file);
    if (entries != nullptr)
        delete[] entries;

    printf("Delete\n");
    if(files_data != nullptr)
    {
        for(unsigned i = 0; i < nb_files; i++)
        {
            if(files_data[i] != nullptr)
            {
                delete[] files_data[i];
            }
        }
    }
    delete[] files_data;
    delete[] files_loaded;

}

/**
 * Returns the stored file's data from giving a filename.
 */
File Archive::get(const char *filename)
{
    for (unsigned index = 0; index < nb_files; index++)
    {
        if (strncmp(filename, entries[index].filename, 8) == 0)
        {
            // On demand load
            if(!files_loaded[index])
            {
                uint8_t *data = new uint8_t[entries[index].file_size];
                fseek(file, files_data_offset[index] + 32 + 24 * nb_files, SEEK_SET);
                if (fread(data, sizeof(uint8_t), entries[index].file_size, file) !=
                    entries[index].file_size)
                {
                    // throw loading error
                }
                else
                {
                    files_data[index] = data;
                    entries[index].data = data;
                }
                files_loaded[index] = true;

            }
            return entries[index];
        }
    }
    return File();
    // throw not found exception
}

File::File(uint8_t *data): data(data)
{

}

File::File() : data(nullptr)
{

}


File::~File()
{
}

const uint8_t* File::get()
{
    return data;
}
