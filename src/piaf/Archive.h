#ifndef INCLUDE_ARCHIVE_H
#define INCLUDE_ARCHIVE_H

#include <fstream>
#include <cstdint>

namespace WalrusRPG
{
    namespace PIAF
    {
        enum File_Type
        {
            UNKNOWN,
            MAP,
            EVENT_LIST,
            TEXT,
            TEXTURE
        };
        enum Compression_Type
        {
            UNKNWOWN,
            RAW,
            ZLIB,
            RLE
        };

        struct File_Entry
        {
            char filename[9]; // 8 + a \0 in case of printing
            File_Type file_type;
            Compression_Type compression_type;
            uint32_t file_size;
            uint32_t data_offset;
        };

        class Archive
        {
          private:
            std::fstream file;
            uint32_t version;
            uint32_t nb_files;
            uint32_t data_size;
            // wouldn't be a map easier to handle for file opening?
            File_Entry *entries;

          public:
            // RAII stuff
            Archive(char *filepath);
            ~Archive();

            File_Entry get_file_entry(char *filename);
            /*PFile*/ void get(char *filename);
        };
    }
}

#endif
