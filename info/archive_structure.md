# Walrus RPG PIAF (PIAF is an Archive Format)
## Control Header
```markdown
WRPG_PIAF_MAGIC_HEADER       : "WRPGPIAF"          <=> 8 bytes (not \0 terminated)
WRPG_PIAF_HEADER_CHECKSUM    :  unsigned int       <=> 4 bytes (This checksum covers the version, number of files and the data size).
WRPG_PIAF_FILETABLE_CHECKSUM :  unsigned int       <=> 4 bytes (This checksum covers the whole filetable data part).
WRPG_PIAF_VERSION            :  unsigned int       <=> 4 bytes (Stored in this form : 0xAABBCCDD, where A = major version, B = minor version, C = hotfix)
WRPG_PIAF_NB_FILES           :  unsigned int       <=> 4 bytes
WRPG_PIAF_DATA_SIZE          :  unsigned int       <=> 4 bytes
<padding>                    :  <filler>           <=> 4 bytes
```
Effective size : 32 bytes.

## Checksum
Checksumps should be done with zlib's crc32. Header checksum covering the last part of the header, the filetable checksum covering the whole filetable data part.

## File table
```markdown
WRPG_PIAF_FILENAME         : char[8]      <=> 8 bytes (not \0 terminated)
WRPG_PIAF_FILE_TYPE        : enumeration  <=> 4 bytes
WRPG_PIAF_COMPRESSION_TYPE : enumeration  <=> 4 bytes
WRPG_PIAF_FILE_SIZE        : unsigned int <=> 4 bytes
WPRG_PIAF_DATA_OFFSET      : unsigned int <=> 4 bytes // To being able to point directly to file data
```
Effective size = 24 bytes per file.

## File types
```markdown
0 = UNKNOWN
1 = MAP
2 = EVENT_LIST
3 = TEXT
4 = TEXTURE (PNG?)
```

## Compression types
```markdown
0 = RAW
1 = ZLIB
2 = RLE (?)
```

## Archive draft
### class / structure
```c++

struct WRPG_PIAF_FILE_ENTRY {
  char filename[9]; // or 9 if you want to put that \0.
  enum WRPG_PIAF_FILE_TYPE file_type;
  enum WRPG_PIAF_COMPRESSION_TYPE file_compresison;
  unsigned int file_size;
  unsigned int data_offset;
}

struct WRPG_PIAF_FILE {
  char magic_header[9];
  unsigned int header_checksum;
  unsigned int file_checksum;
  unsigned int version;
  unsigned int nb_files;
  unsigned int data_size;
  unsigned int padidng;
  WRPG_PIAF_FILE_ENTRY* entries;
};

```
### Loading
Here's the current state of the parser. Working on a x64 Linux. It should work everywhere because it doesn't care about endianess anymore.

Take note that it'll read big-endian only, so work on the file writer will have to be done to make it universal.
```c++
#include <iostream>
#include <cstring>

#define CURRENT_PIAF_VERSION 0x00000000
enum WRPG_PIAF_FILE_TYPE { FT_UNKNOWN, MAP, EVENT_LIST, TEXT, SPRITESHEET };
enum WRPG_PIAF_COMPRESSION_TYPE { COMP_UNKNOWN, RAW, ZLIB, RLE };

struct WRPG_PIAF_FILE_ENTRY {
  char filename[9]; // or 9 if you want to put that \0.
  enum WRPG_PIAF_FILE_TYPE file_type;
  enum WRPG_PIAF_COMPRESSION_TYPE file_compression;
  unsigned file_size;
  unsigned data_offset;
};

struct WRPG_PIAF_FILE {
  char magic_header[9];
  unsigned long long checksum;
  // 0xAABBCCCC form
  // AA = major version, BB = minor version, CC = hotfix
  unsigned version;
  unsigned nb_files;
  unsigned data_size;
  unsigned padding;
  WRPG_PIAF_FILE_ENTRY *entries;
};

unsigned read_unsigned(void *ptr) {
  unsigned result = 0;
  char *data = (char *)ptr;
  for (int i = 0; i < sizeof(unsigned); i++)
    result |= data[i] << (8 * (sizeof(unsigned) - 1 - i));
  return result;
}
unsigned read_unsigned_long_long(void *ptr) {
  unsigned long long result = 0;
  char *data = (char *)ptr;
  for (int i = 0; i < sizeof(unsigned long long); i++)
    result |= data[i] << (8 * (sizeof(unsigned long long) - 1 - i));
  return result;
}

struct WRPG_PIAF_FILE *load_structure_from_data(void *data, ssize_t size) {
  if (data == nullptr || size < 32) {
    fprintf(stderr, "Bad pointer/size\n");
    return nullptr; // NPE
  }

  char *data_char = (char *)data;
  if (strncmp(data_char, "WRPGPIAF", 8) != 0) {
    // BAD_COOKIE exception
    fprintf(stderr, "Bad version\n");
    return nullptr;
  }

  unsigned int header_checksum = read_unsigned(&data_char[8]);
  unsigned int filetable_checksum = read_unsigned(&data_char[12]);

  bool check_ok = true;
  // checksum routine goes here
  if (!check_ok) {
    fprintf(stderr, "Bad checksum\n");
    // BAD_CHECKSUM exception
    return nullptr;
  }

  unsigned file_version = read_unsigned(&data_char[16]);
  if (file_version != CURRENT_PIAF_VERSION) {
    fprintf(stderr, "Bad file version\n");
    // BAD_VERSION exception
    // Or manage older versions.
    return nullptr;
  }

  unsigned nb_files = read_unsigned(&data_char[20]);
  printf("NB FILES : %d\n", nb_files);
  unsigned data_size = read_unsigned(&data_char[24]);

  if (data_size != size - 32 - 24 * nb_files) {
    // BAD_SIZE exception
    printf("Data size mismatch\n");
    return nullptr;
  }

  struct WRPG_PIAF_FILE_ENTRY *file_table = nullptr;
  if (nb_files > 0) {
    file_table = (WRPG_PIAF_FILE_ENTRY *)malloc(
        nb_files * sizeof(struct WRPG_PIAF_FILE_ENTRY));
    if (file_table == nullptr) {
      fprintf(stderr, "NPE Filetable\n");
      // NO_MORE_MEMORY exception
      return nullptr;
    }
    // I don't know if that works. It should because these structs are POD but
    // well...
    memcpy(file_table, &data_char[32], sizeof(WRPG_PIAF_FILE_ENTRY) * nb_files);
  }
  struct WRPG_PIAF_FILE *file =
      (WRPG_PIAF_FILE *)malloc(sizeof(struct WRPG_PIAF_FILE));
  if (file == nullptr) {
    // NO_MORE_MEMORY exception
    free(file_table);
    return nullptr;
  }

  // Now the structure is parsed.

  memcpy(file->magic_header, data_char, 8);
  for (size_t i = 0; i < nb_files; i++) {
    char *file_entry = &data_char[32 + 24 * i];
    memcpy(file_table[i].filename, file_entry, 8);
    file_table[i].filename[8] = '\0'; // Making sure that there is a terminator.
    file_table[i].file_type =
        static_cast<WRPG_PIAF_FILE_TYPE>(read_unsigned(&file_entry[8]));
    file_table[i].file_compression =
        static_cast<WRPG_PIAF_COMPRESSION_TYPE>(read_unsigned(&file_entry[12]));
    file_table[i].file_size = read_unsigned(&file_entry[16]);
    file_table[i].data_offset = read_unsigned(&file_entry[20]);
  }
  file->magic_header[8] = '\0';
  file->checksum = checksum;
  file->version = file_version;
  file->nb_files = nb_files;
  file->data_size = data_size;
  file->padding = 0XDEADBEEF;
  file->entries = file_table;

  return file;
}

int main(int argc, char const *argv[]) {
  /* code */
  char data[] = {'W', 'R', 'P', 'G', 'P', 'I', 'A', 'F', /*checksum*/ 0, 0, 0,
                 0, 0, 0, 0, 0, /*version*/ 0, 0, 0, 0, /*nb_files*/ 0, 0, 0, 2,
                 /*data size*/ 0, 0, 0, 1, /*padding*/ 0, 0, 0, 0,
                 /*File 1*/ 'T', 'e', 's', 't', 'i', 'n', 'g', '\0',
                 /*filetype*/ 0, 0, 0, MAP, /*file compression*/ 0, 0, 0, RAW,
                 /*filesize*/ 0, 0, 0, 1, /*offset*/ 0, 0, 0, 0,
                 /*File 2*/ 'T', 'e', 's', 't', 'i', 'n', 'g', '2',
                 /*filetype*/ 0, 0, 0, MAP, /*file compression*/ 0, 0, 0, RAW,
                 /*filesize*/ 0, 0, 0, 0, /*offset*/ 0, 0, 0, 0,
                 /* data*/ 42

  };
  WRPG_PIAF_FILE *archive = load_structure_from_data(data, sizeof(data));
  if (archive == nullptr) {
    printf("Failed\n");
  } else {
    printf("Result\n");
    printf("magic_header : %s\n", archive->magic_header);
    printf("checksum : 0x%llx\n", archive->checksum);
    printf("version : 0x%08x\n", archive->version);
    printf("nb_files : %u\n", archive->nb_files);
    printf("data_size : %u\n", archive->data_size);
    printf("entries : %x\n", archive->entries);
    for (size_t i = 0; i < archive->nb_files; i++) {
      printf("> File N°%d\n", i);
      printf(">   name : %s\n", archive->entries[i].filename);
      printf(">   file_type : %u\n", archive->entries[i].file_type);
      printf(">   file_size : %u\n", archive->entries[i].file_size);
      printf(">   data_offset : %u\n", archive->entries[i].data_offset);
      /* code */
    }
  }
  return 0;
}
```
