```markdown
MAP_MAGIC_HEADER    : 'WMAP'          <=> 4 bytes
MAP_HEADER_CHECKSUM : unsigned int    <=> 4 bytes
MAP_DATA_SIZE       : unsigned int    <=> 4 bytes
MAP_WIDTH           : unsigned short  <=> 2 bytes
MAP_HEIGHT          : unsigned short  <=> 2 bytes
MAP_N_LAYERS        : unsigned short  <=> 2 bytes
MAP_N_EVENTS        : unsigned short  <=> 2 bytes
MAP_DATA_COMPRESSION     : enumeration     <=> 4 bytes (IIRC, unless we force using some sort of enum class and use a smaller size)
  - 0 : RAW
  - 1 : RLE_PER_LAYER (stop when a layer ends)
  - 2 : RLE_ALL_LAYERS (stop at the end of the last layer)
  - 3 : ZLIB (?)
  - 4 : ...
MAP_DATA            : variable size   <=> n*4 bytes
MAP_EVENT_DATA      : MAP_N_EVENTS * sizeof(Event) <=> ?
// MAP_CHECKSUM?
Size for a map of n*m * l layers = 24 bytes + up to n*m*l*4 bytes
```

#TODO
- Add tilemap link (via filename in same archive?)
- Determine what's an event.

# Outdated source draft
**Must update this before copy-pasting it. CHanges has been done to include checksum and events.**
Loader draft:
```c++
#include <cstring>
  Map load_map(void* data, uint32_t datasize) throw MapLoadException {
    uint32_t expected_checksum, real_checksum;
    uint32_t expected_data_size, real_data_size;
    uint16_t map_width = 0, map_height = 0, map_n_layers = 0, map_n_events;
    enum Map_Compression map_compression = RAW; // or whatever Map_Compression will be

    char* cdata = (char*) data;
    if(cdata == NULL) throw NPE(); // Null Pointer Exception

    if (datasize < 24
    {
      throw MLE("File too small for header.");
    }

    if(strncmp(cdata, "WMAP", 4)) throw MLE("Bad map header.");

    expected_checksum = read_big_endian_value<uint32_t>(&cdata[4]);
    real_checksum = crc32(0L, &cdata[12], 16);
    if(expected_checksum != real_checksum)
    {
      throw MLE("Bad checksum");
    }

    expected_data_size = read_big_endian_value<uint32_t>(&cdata[16]);
    real_data_size = datasize - 24;
    if(expected_data_size != real_data_size)
    {
      throw MLE("Bad data size");      
    }

    map_witdh = read_big_endian_value<uint32_t>(&cdata[8]);
    map_height = read_big_endian_value<uint32_t>(&cdata[12]);

    map_n_layers = read_big_endian_value<uint32_t>(&cdata[16]);
    if(map_n_layers < 1 || map_n_layers > 2) thorw MLE("Wrong map layer number.");

    map_n_events = read_big_endian_value<uint32_t>(&cdata[20]);
    if(map_n_events != 0) thorw MLE("Events not supported yet.");

    map_compression = read_big_endian_value<uint32_t>(&cdata[24]);

    unsigned *layer0, *layer1;
    switch(map_compression) {
      // copy layers from &udata[6] and &udata[6*(map_width*map_height/sizeof(unsigned))]
      case RAW:
      // TODO
      break;
      case RLE_PER_LAYER:
      // TDOO
      break;
      case RLE_ALL_LAYERS:
      // TODO
      break;
      case ZLIB:
      // TODO
      break;
      default:
        throw MLE("Wrong compression");
    }

    return Map(map_width, map_height, layer0, layer1);
  }
