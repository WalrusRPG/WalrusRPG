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
