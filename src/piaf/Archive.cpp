#include "Archive.h"
#include <cstring>

using WalrusRPG::PIAF::Archive;
using WalrusRPG::PIAF::FileEntry;
using WalrusRPG::PIAF::FileType;

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

FileEntry Archive::get_file_entry(char *filename)
{
    for (uint32_t index = 0; index < nb_files; index++)
    {
        if (strcmp(entries[index].filename, filename) == 0)
        {
            return entries[index];
        }
    }
    // throw exception
}

/*PFile*/ void /*FileEntry*/ get(char *filename)
{
    // return PFile(get_file_entry(filename));
}
