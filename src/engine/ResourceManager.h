#ifndef INCLUDE_RESOURCEMANAGER_H
#define INCLUDE_RESOURCEMANAGER_H

#include "piaf/Archive.h"

namespace WalrusRPG
{
    /**
     * A ManagedArchive is an Archive managed by the RessourceManager.
     * It acts as a RAII wrapper of the ResourceManager's require/release
     * functions.
     */
    class ManagedArchive
    {
      protected:
        const char *path;
        PIAF::Archive *arc;

      public:
        ManagedArchive(const char *path);
        ~ManagedArchive();
        // Small utility to use the Managed archive as an Archive.
        operator PIAF::Archive *() const;
    };

    namespace ResourceManager
    {
        // Call this to init the RM.
        void init();
        // Call this before program exit to clean leftovers.
        void deinit();
        // Ask for a file.
        PIAF::Archive *require(const char *path);
        // Call this to release the required resource. Actually, use ManagedArchive
        // instead as it wraps the ResourceManager
        void release(WalrusRPG::PIAF::Archive *arcs);
        // Call this to release the required resource. Actually, use ManagedArchive
        // instead as it wraps the ResourceManager
        void release(const char *path);
    }
}

#endif
