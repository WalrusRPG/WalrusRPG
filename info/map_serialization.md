```markdown
MAP_MAGIC_HEADER : 'WRPG_MAP'    <=> 8 bytes
MAP_WIDTH        : unsigned int  <=> 4 bytes
MAP_HEIGHT       : unsigned int  <=> 4 bytes
MAP_N_LAYERS     : unsigned char <=> 4 byte
MAP_COMPRESSION  : enumeration   <=> 4 bytes (IIRC, unless we force using some sort of enum class and use a smaller size)
  - 0 : RAW
  - 1 : RLE_PER_LAYER (stop when a layer ends)
  - 2 : RLE_ALL_LAYERS (stop at the end of the last layer)
  - 3 : ZLIB (?)
  - 4 : ...
MAP_DATA         : variable size <=> n*4 bytes
// MAP_CHECKSUM?
Size for a map of n*m * l layers = 24 bytes + up to n*m*l*4 bytes
```

Loader draft:
```c++
struct Map_Data {
    char magic_string[8];
    unsigned width : 4;
    unsigned height : 4;
    unsigned n_layers : 4;
    enum Map_Compression compression;
};

#include <cstring>
  Map load_map(void* data) throw MapLoadException {
    unsigned map_width = 0, map_height = 0, map_n_layers = 0;
    enum Map_Compression map_compression = RAW; // or whatever Map_Compression will be
    char* cdata = (char*) data;
    unsigned* udata = (unsigned*) data;
    if(cdata == NULL) throw NPE(); // Null Pointer Exception
    if(strncmp(cdata, "WRPG_MAP", 8)) throw MLE("Bad map header.");
    // Assuming big endian
    map_width = ((cdata[8]&7) << 24) | ((cdata[9]&7) << 16) | ((cdata[10]&7) << 8) | (((cdata[11]&7)));
    map_height = ((cdata[12]&7) << 24) | ((cdata[13]&7) << 16) | ((cdata[14]&7) << 8) | (((cdata[15]&7)));
    map_n_layers = ((cdata[16]&7) << 24) | ((cdata[17]&7) << 16) | ((cdata[18]&7) << 8) | (((cdata[19]&7)));
    if(map_n_layers < 1 || map_n_layers > 2) thorw MLE("Wrong map layer number.");

    map_compression = ((cdata[20]&7) << 24) | ((cdata[21]&7) << 16) | ((cdata[22]&7) << 8) | (((cdata[23]&7)));

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
```