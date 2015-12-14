#ifndef INCLUDE_ARCHIVE_H
#define INCLUDE_ARCHIVE_H

#include <exception>
#include <cstdint>
#include <cstdio>
#include <TINYSTL/string.h>

namespace WalrusRPG
{
    namespace PIAF
    {
        constexpr uint32_t ARCHIVE_VERSION = 0x01000000;
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
            const uint8_t *data;

          public:
            friend class Archive;
            char filename[9]; // 8 + a \0 in case of printing
            FileType file_type;
            CompressionType compression_type;
            uint32_t file_size;
            std::size_t size;

            File(uint8_t *data);
            File();
            ~File();
            const uint8_t *get();
        };

        class Archive
        {
          private:
            FILE *file;
            uint32_t version;
            uint32_t nb_files;
            uint32_t data_size;
            File *entries;
            uint8_t **files_data;
            bool *files_loaded;
            uint32_t *files_data_offset;


          public:
            // RAII stuff
            // Archive(std::unique_ptr<char> *filepath);
            Archive(const char *filepath);
            Archive(tinystl::string &filepath);
            ~Archive();
            bool has(const char *filename);
            File get(const char *filename);
        };

        class PIAFException : public std::exception
        {
            private:
                char *msg;

            public:
            PIAFException(const char *file, const unsigned line, const char *message);
            virtual ~PIAFException();

            const char* what() const throw();
        };

    }
}

#endif
