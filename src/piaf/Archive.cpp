#include <cstring>
#include <cstdio>
#include <memory>
#include <zlib.h>
#if NSPIRE
#include "../../platform/nspire/Interrupts.h"
#endif
#include "Archive.h"
#include "Quirks.h"
#include "utility/misc.h"

using tinystl::string;
using WalrusRPG::PIAF::ARCHIVE_VERSION;
using WalrusRPG::PIAF::Archive;
using WalrusRPG::PIAF::File;
using WalrusRPG::PIAF::FileType;
using WalrusRPG::PIAF::CompressionType;
using namespace WalrusRPG::PIAF;

#if NSPIRE
using namespace Nspire;
#endif
namespace
{
    // Must get a pointer on the file table.
    /**
     * Reads the file table from given data pointer to an empty FileEntry
     * array long enough and a given number of files to load.
     * The pointer must directly access the file entry region of the archive.
     */
    void load_file_table(File *entries, uint32_t *data_offsets, char *data,
                         uint32_t nb_files)
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


Archive::Archive(const char *filepath)
    : file(nullptr), entries(nullptr), files_data(nullptr), files_loaded(nullptr)
{
#if NSPIRE
    Interrupts::off();
#endif
    // Null pointer exception trigger
    if (filepath == nullptr)
    {
        throw PIAF::PIAFException("%s: Null path given", __FILE__);
    }
    // Solves the absolute path for given relative path.
    // Must be needed in targets like Ndless as it doesn't support environment
    // vars and thus PATH.
    std::unique_ptr<char> real_filename(Quirks::solve_absolute_path(filepath));

    // Open the archive
    file = fopen(real_filename.get(), "rb");
    // Again another null pointer trigger
    if (file == nullptr || file == NULL)
    {
        throw PIAF::PIAFException("%s: Missing file : %s", __FILE__, filepath);
    }

    // Loading stuff happens NOW
    // Checking if the file is long enough to have a header
    fseek(file, 0L, SEEK_END);
    uint64_t filesize = ftell(file);
    fseek(file, 0L, SEEK_SET);
    // File to small exception trigger
    if (filesize < 32)
    {
        throw PIAF::PIAFException("%s: File too small (%s): %d", __FILE__, filepath,
                                  filesize);
    }

    // Tempoary buffer to contain the header.
    char header_container[32] = {0};
    // Read the headers and trigger exceptions on errors
    if (fread(header_container, sizeof(char), 32, file) != 32)
    {
        throw PIAF::PIAFException("%s: Errorneous header : %s", __FILE__, filepath);
    }
    // Check if the magic cookie is the same.
    // It's a first way to detect if the file is correctly an archive.
    if (strncmp(header_container, "WRPGPIAF", 8) != 0)
    {
        // TODO throw bad header
        // fprintf(stderr, "Bad header magic word\n");
        throw PIAF::PIAFException("%s: Magic cookie mismatch : %s", __FILE__, filepath);
    }
    // Checksum time! Let's check if the header hasn"t been altered.
    uint32_t expected_checksum = read_big_endian_value<uint32_t>(&header_container[8]);
    uint32_t calculated_checksum = crc32(0L, (unsigned char *) &header_container[16], 16);
    if (expected_checksum != calculated_checksum)
    {
        // TODO throw bad checksum
        // fprintf(stderr, "Bad header checksum : %x != %x\n", expected_checksum,
        // calculated_checksum);
        throw PIAF::PIAFException("%s: Bad checksum : %s", __FILE__, filepath);
    }

    // TODO : version checking
    version = read_big_endian_value<uint32_t>(&header_container[16]);
    if (version != ARCHIVE_VERSION)
    {
        // std::exception up;
        // throw up; // haha
        throw PIAF::PIAFException("%s: Wrong(%s) : %08x is not supported by %08x",
                                  __FILE__, filepath, version, ARCHIVE_VERSION);
    }


    // At this point, the archive header looks unaltered and we finally can parse
    // and load the header.

    // Read the archive's number of files
    nb_files = read_big_endian_value<uint32_t>(&header_container[20]);
    // printf("nb_files : %u\n", nb_files);

    data_size = read_big_endian_value<uint32_t>(&header_container[24]);
    uint64_t calculated_data_size = filesize - 32 - 24 * nb_files;
    if (data_size != calculated_data_size)
    {
        // fprintf(stderr, "Bad data size : expected %u, got %lld\n", data_size,
        // calculated_data_size);
        throw PIAF::PIAFException("Data size mismatch", __LINE__, filepath);
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
        if (fread(file_entry_data, sizeof(char), 24 * nb_files, file) < 24 * nb_files)
        {
            throw PIAF::PIAFException("Can't read file entry data", __LINE__, filepath);
        }
        // Compare and trigger an exception if the checksum doesn't match.
        if (expected_filetable_checksum !=
            crc32(0L, (unsigned char *) file_entry_data, 24 * nb_files))
        {
            // fprintf(stderr, "Bad filetable checksum\n");
            throw PIAF::PIAFException("Bad Filetable checksum", __LINE__, filepath);
        }
        // Create the filetable.
        entries = new File[nb_files];
        // Parse and story the filetable.

        files_data = new uint8_t *[nb_files];
        files_loaded = new bool[nb_files];
        files_data_offset = new uint32_t[nb_files];
        for (unsigned i = 0; i < nb_files; i++)
        {
            files_data[i] = nullptr;
            files_loaded[i] = false;
            files_data_offset[i] = 0;
        }

        load_file_table(entries, files_data_offset, file_entry_data, nb_files);
        delete[] file_entry_data;
    }
#if NSPIRE
    Interrupts::init();
#endif
}

Archive::~Archive()
{
    if (file != nullptr)
        fclose(file);
    if (entries != nullptr)
        delete[] entries;

    if (files_data != nullptr)
    {
        for (unsigned i = 0; i < nb_files; i++)
        {
            if (files_data[i] != nullptr)
            {
                delete[] files_data[i];
            }
        }
    }
    delete[] files_data;
    delete[] files_loaded;
    delete[] files_data_offset;
}

bool Archive::has(const char *filename)
{
    for (unsigned index = 0; index < nb_files; index++)
        if (strncmp(filename, entries[index].filename, 8) == 0)
            return true;
    return false;
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
            if (!files_loaded[index])
            {
#if NSPIRE
                Interrupts::off();
#endif
                uint8_t *data = new uint8_t[entries[index].file_size];
                fseek(file, files_data_offset[index] + 32 + 24 * nb_files, SEEK_SET);
                if (fread(data, sizeof(uint8_t), entries[index].file_size, file) !=
                    entries[index].file_size)
                {
                    throw PIAF::PIAFException("%s: couldn't load %s from an archive.",
                                              filename);
                }
                else
                {
                    files_data[index] = data;
                    entries[index].data = data;
                }
                files_loaded[index] = true;
#if NSPIRE
                Interrupts::init();
#endif
            }
            return entries[index];
        }
    }
    return File();
}

File::File(uint8_t *data) : data(data)
{
}

File::File() : data(nullptr)
{
}


File::~File()
{
}

const uint8_t *File::get()
{
    return data;
}
