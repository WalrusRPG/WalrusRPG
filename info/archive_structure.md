# Walrus RPG PIAF (PIAF is an Archive Format)
## Control Header
```markdown
WRPG_PIAF_MAGIC_HEADER : "WRPGPIAF"          <=> 8 bytes (not \0 terminated)
WRPG_PIAF_CHECKSUM     :  unsigned long long <=> 8 bytes
WRPG_PIAF_VERSION      :  unsigned int       <=> 4 bytes
WRPG_PIAF_NB_FILES     :  unsigned int       <=> 4 bytes
WRPG_PIAF_DATA_SIZE    :  unsigned int       <=> 4 bytes
<padding>              :  <filler>           <=> 4 bytes
```
Effective size : 32 bytes.

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
4 = SPRITESHEET
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
  char filename[8]; // or 9 if you want to put that \0.
  enum WRPG_PIAF_FILE_TYPE file_type;
  enum WRPG_PIAF_COMPRESSION_TYPE file_compresison;
  unsigned int file_size;
  unsigned int data_offset;
}

struct WRPG_PIAF_FILE {
  char magic_header[8];
  unsigned long long checksum;
   // 0xAABBCCCC form
   // AA = major version, BB = minor version, CC = hotfix
  unsigned int version;
  unsigned int nb_files;
  unsigned int data_size;
  unsigned int padidng;
  WRPG_PIAF_FILE_ENTRY* entries;
};

```
### Loading
```c++
// Does only load structure data. Gotta use loader functions to
// load from this structure.
struct WRPG_PIAF_FILE* load_structure_from_data(void* data, ssize_t size) {
  if(data == nullptr)
    return nullptr; // NPE

  char* data_char = (char*) data;
  if(strncmp(data_char, "WRPGPIAF", 8) != 0)
    // BAD_COOKIE exception
    return nullptr;

  unsigned long long checksum;
  memcpy(&checksum, &data_char[8], sizeof(unsigned long long));
  bool check_ok = false;
  // checksum routine goes here
  if(!check_ok)
    // BAD_CHECKSUM exception
    return nullptr;

  unsigned file_version;
  memcpy(&file_version, &data_char[16], sizeof(unsigned));
  if(file_version != CURRENT_PIAF_VERSION)
    // BAD_VERSION exception
    // Or manage older versions.
    return nullptr;


  unsigned nb_files;
  memcpy(&nb_files, &data_char[20], sizeof(unsigned));

  unsigned data_size;
  memcpy(&data_size, &data_char[24], sizeof(unsigned));
  if(data_size != size - 32 - 24*nb_files)
    // BAD_SIZE exception
    return nullptr;

  struct WRPG_PIAF_FILE_ENTRY *file_table = malloc(nb_files * sizeof(struct WRPG_PIAF_FILE_ENTRY));
  if(file_table == NULL)
    // NO_MORE_MEMORY exception
    return nullptr;
  struct WRPG_PIAF_FILE *file = malloc(sizeof(struct WRPG_PIAF_FILE));
  if(file == NULL) {
    // NO_MORE_MEMORY exception
    free(file_table);
    return nullptr;
  }

  // I don't know if that works. It should because these structs are POD but well...
  for (unsigned i = 0; i < nb_files; i++) {
    memcpy(&file_table[i], &data_char[32+sizeof(WRPG_PIAF_FILE_ENTRY)*i]);
  }
  file_table[i].data = nullptr;

  // Now the structure is parsed.

  memcpy(file.magic_header, data_char, 8);
  file.checksum = checksum;
  file.version = file_version;
  file.nb_files = nb_files;
  file.data_size = data_size;
  file.padding = 0XDEADBEEF;
  file.entries = file_table;

  return file;

}
```
