# Map Serialization

```markdown
MAP_MAGIC_HEADER     : 'WMAP'          <=> 4 bytes
MAP_HEADER_CHECKSUM  : unsigned int    <=> 4 bytes
MAP_VERSION          : unsigned int    <=> 4 bytes
MAP_DATA_SIZE        : unsigned int    <=> 4 bytes
MAP_WIDTH            : unsigned short  <=> 2 bytes
MAP_HEIGHT           : unsigned short  <=> 2 bytes
MAP_N_LAYERS         : unsigned short  <=> 2 bytes
# TODO : This is moving to the meta-map file, alongside the objects and metadata.
MAP_N_EVENTS         : unsigned short  <=> 2 bytes
MAP_DATA_COMPRESSION : enumeration     <=> 4 bytes (IIRC, unless we force using some sort of enum class and use a smaller size)
  - 0 : UNKWOWN
  - 1 : RAW
  - 2 : RLE_PER_LAYER (stop when a layer ends)
  - 3 : RLE_ALL_LAYERS (stop at the end of the last layer)
  - 4 : ZLIB (?)
  - 4 : ...
MAP_DATA            : variable size   <=> n*4 bytes
# TODO : This is moving to the meta-map file, alongside the objects and metadata.
MAP_EVENT_DATA      : MAP_N_EVENTS * sizeof(Event) <=> ?
// MAP_CHECKSUM?
Size for a map of n*m * l layers = 28 bytes + up to n*m*l*4 bytes
```

## TODO
- Add tilemap link (via filename in same archive?)
- Determine what's an event.

## Outdated source draft
**Check Map.cpp for a up to date implementation.**
Loader draft:
```c++
#include <cstring>
Map load_map(void* data, uint32_t datasize) throw MapLoadException {
  uint32_t map_version;
  uint32_t expected_checksum, real_checksum;
  uint32_t expected_data_size, real_data_size;
  uint16_t map_width = 0, map_height = 0, map_n_layers = 0, map_n_events;
  enum Map_Compression map_compression = RAW; // or whatever Map_Compression will be

  char* cdata = (char*) data;
  if(cdata == NULL) throw NPE(); // Null Pointer Exception

  if (datasize < 28)
  {
    throw MLE("File too small for header.");
  }

  if(strncmp(cdata, "WMAP", 4)) throw MLE("Bad map header.");

  expected_checksum = read_big_endian_value<uint32_t>(&cdata[4]);
  real_checksum = crc32(0L, &cdata[8], 20);
  if(expected_checksum != real_checksum)
  {
    throw MLE("Bad checksum");
  }

  version = read_big_endian_value<uint32_t>(&cdata[8]);
  // TODO : version check

  expected_data_size = read_big_endian_value<uint32_t>(&cdata[12]);
  real_data_size = datasize - 24;
  if(expected_data_size != real_data_size)
  {
    throw MLE("Bad data size");      
  }

  map_witdh = read_big_endian_value<uint16_t>(&cdata[16]);
  map_height = read_big_endian_value<uint16_t>(&cdata[18]);

  map_n_layers = read_big_endian_value<uint16_t>(&cdata[20]);
  if(map_n_layers < 1 || map_n_layers > 2) throw MLE("Wrong map layer number.");

  map_n_events = read_big_endian_value<uint16_t>(&cdata[22]);
  if(map_n_events != 0) thorw MLE("Events not supported yet.");

  map_compression = read_big_endian_value<uint32_t>(&cdata[24]);

  unsigned *layer0, *layer1;
  switch(map_compression) {
    // copy layers from &udata[6] and &udata[6*(map_width*map_height/sizeof(uint32_t))]
    case RAW:
    // TODO
    break;
    /* // NOT IMPLEMENTED YET //
    case RLE_PER_LAYER:
    // TDOO
    break;
    case RLE_ALL_LAYERS:
    // TODO
    break;
    case ZLIB:
    // TODO
    break;
    */
    default:
      throw MLE("Wrong compression");
  }

  return Map(map_width, map_height, layer0, layer1);
}
// **
```

# Tileset Serialization

```markdown
SET_MAGIC_HEADER     : 'WTst'          <=> 4 bytes
SET_HEADER_CHECKSUM  : unsigned int    <=> 4 bytes
SET_VERSION          : unsigned int    <=> 4 bytes
SET_SHEET_FILENAME   : char[8]         <=> 8 bytes
SET_SPRITE_SIZE      : unsigned short  <=> 2 bytes
SET_N_TILES          : unsigned short  <=> 2 bytes
---
TILES_DATA           : struct          <=> n bytes
  - COLLISION_MASK   : unsigned char   <=> 1 byte
Size for a tilemap of n tiles = 24 bytes + up to 1 * n bytes
```

Tilemap Loader draft:

```c++
struct Tilechip {
  uint8_t collision_mask;
};

constexpr unsigned TILESET_CHIP_DATA_SIZE = 1;
constexpr unsigned TILESET_HEADER_SIZE = 24;

#include <cstring>
Tilemap load_tilemap(void* data, uint32_t datasize) {
  uint32_t expected_checksum, real_checksum;
  uint32_t set_version;
  uint32_t set_sprite_size;
  char sheet_filename[8] = {0};
  Tilechip *chips;

  char* cdata = (char*) data;
  if(cdata == NULL) throw TLE(); // Null Pointer Exception

  if (datasize < 24)
  {
    throw TLE("File too small for header.");
  }
  if(strncmp(cdata, "WTST", 4)) throw TLE("Bad map header.");

  expected_checksum = read_big_endian_value<uint32_t>(&cdata[4]);
  real_checksum = crc32(0L, &cdata[8], 16);
  if(expected_checksum != real_checksum)
  {
    throw TLE("Bad checksum");
  }

  version = read_big_endian_value<uint32_t>(&cdata[8]);
  // TODO : version check
  memcpy(sheet_filename, &cdata[12], sizeof(unsigned char)*8);
  set_sprite_size = read_big_endian_value<uint32_t>(&cdata[20]);
  set_n_tiles = read_big_endian_value<uint32_t>(&cdata[22]);

  chips = new Tilechip[set_n_tiles];

  for(int i = ); i < set_n_tiles; i++) {
    chips[i].collision_mask = read_big_endian_value<uint8_t>(&cdata[TILESET_HEADER_SIZE + i*TILESET_CHIP_DATA_SIZE]);
  }

}
```
