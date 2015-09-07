#include "Archive.h"
#include <cstring>

using WalrusRPG::PIAF::Archive;
using WalrusRPG::PIAF::File_Entry;
using WalrusRPG::PIAF::File_Type;

Archive::Archive(char *filepath)
{
    // loading stuff happens NOW
    // ...
    // entries = new File_Entry[];
    // ...
}

Archive::~Archive()
{
    // delete[] entries;
}

File_Entry Archive::get_file_entry(char *filename)
{
    for (uint32_t index = 0; index < nb_files; index++) {
        if(strcmp(entries[index].filename, filename) == 0) {
            return entries[index];
        }
    }
    // throw exception
}

/*PFile*/ void /*File_Entry*/get(char*filename)
{
    // return PFile(get_file_entry(filename));
}
