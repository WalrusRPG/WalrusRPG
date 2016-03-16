#ifndef INCLUDE_RESOURCEMANAGER_H
#define INCLUDE_RESOURCEMANAGER_H

#include "piaf/Archive.h"

namespace WalrusRPG
{
	class ManagedArchive {
		protected:
			const char *path;
			PIAF::Archive* arc;
		public:
		ManagedArchive(const char *path);
		~ManagedArchive();
		operator PIAF::Archive*() const;
	};

	namespace ResourceManager
	{
		void init();
		void deinit();
		PIAF::Archive* require(const char *path);
		void release(WalrusRPG::PIAF::Archive* arcs);
		void release(const char *path);
	}
}

#endif