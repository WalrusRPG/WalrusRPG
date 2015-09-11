#ifndef INCLUDE_ARCHIVE_H
#define INCLUDE_ARCHIVE_H

#include <fstream>
#include <cstdint>

namespace WalrusRPG
{
    namespace PIAF
    {
        enum FileType : uint32_t
        {
            UNKNOWN,
            MAP,
            EVENT_LIST,
            TEXT,
            TEXTURE
        };
        enum CompressionType : uint32_t
        {
            UNKNWOWN,
            RAW,
            ZLIB,
            RLE
        };

        struct FileEntry
        {
            char filename[9]; // 8 + a \0 in case of printing
            FileType file_type;
            CompressionType compression_type;
            uint32_t file_size;
            uint32_t data_offset;
        };

        class Archive
        {
          private:
            FILE* file;
            uint32_t version;
            uint32_t nb_files;
            uint32_t data_size;
            // wouldn't be a map easier to handle for file opening?
            FileEntry *entries;

          public:
            // RAII stuff
            Archive(char *filepath);
            ~Archive();

            FileEntry get_file_entry(char *filename);
            /*PFile*/ void get(char *filename);
        };
    }
}

#endif
