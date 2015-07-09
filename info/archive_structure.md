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
WRPG_PIAF_FILETYPE         : enumeration  <=> 4 bytes
WRPG_PIAF_COMPRESSION_TYPE : enumeration  <=> 4 bytes
WRPG_PIAF_FILESIZE         : unsigned int <=> 4 bytes
WPRG_PIAF_DATA_OFFSET      : unsigned int <=> 4 bytes
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
