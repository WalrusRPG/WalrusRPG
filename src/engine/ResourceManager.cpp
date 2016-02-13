#include "ResourceManager.h"
#include "TINYSTL/unordered_map.h"

using namespace WalrusRPG; /*::ResourceManager*/
using WalrusRPG::PIAF::Archive;

using tinystl::unordered_map;

static unordered_map<const char *, Archive*> files;

void ResourceManager::init()
{
}

void ResourceManager::deinit()
{
	for(auto it : files){
		delete it.second;
	}
}

Archive& ResourceManager::require(const char *path)
{
	auto entry = files.find(path);
	if(entry == files.end())
	{
		return *files.insert({path, new Archive(path)}).first->second;
		// return nullptr;
	}
	return *entry->second;
}