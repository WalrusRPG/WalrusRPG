#ifndef INCLUDE_ARCHIVE_H
#define INCLUDE_ARCHIVE_H

#include <cstdint>
#include <cstdio>
#include <TINYSTL/string.h>

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

        class File
        {
        protected:
            uint8_t *data;
            bool loaded;
          public:
            friend class Archive;
            char filename[9]; // 8 + a \0 in case of printing
            FileType file_type;
            CompressionType compression_type;
            uint32_t file_size;
            uint32_t data_offset;
            std::size_t size;

            File();
            ~File();
            uint8_t* get();
        };

        class Archive
        {
          private:
            FILE *file;
            uint32_t version;
            uint32_t nb_files;
            uint32_t data_size;
            // wouldn't be a map easier to handle for file opening?
            File *entries;

          public:
            // RAII stuff
            // Archive(std::unique_ptr<char> *filepath);
            Archive(const char *filepath);
            Archive(tinystl::string &filepath);
            ~Archive();
            File& get(char *filename);

        };
    }
}

#endif
