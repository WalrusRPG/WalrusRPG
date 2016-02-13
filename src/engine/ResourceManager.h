#ifndef INCLUDE_RESOURCEMANAGER_H
#define INCLUDE_RESOURCEMANAGER_H

#include "piaf/Archive.h"

namespace WalrusRPG
{
	namespace ResourceManager
	{
		void init();
		void deinit();
		WalrusRPG::PIAF::Archive& require(const char *path);
		void free(const char *path);
		void free(WalrusRPG::PIAF::Archive& arcs);
	}
}

#endif