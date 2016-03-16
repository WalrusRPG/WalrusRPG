#include "ResourceManager.h"
#include "TINYSTL/unordered_map.h"

using namespace WalrusRPG; /*::ResourceManager*/
using WalrusRPG::PIAF::Archive;

using tinystl::unordered_map;

ManagedArchive::ManagedArchive(const char *path): path(path), arc(ResourceManager::require(path))
{
	printf("+Ref %s\n", path);
}
ManagedArchive::~ManagedArchive()
{
	printf("-Ref %s\n", path);
	ResourceManager::release(path);
}

ManagedArchive::operator Archive*() const
{
	return arc;
}

struct node{Archive* arc; uint16_t refcount;};
static unordered_map<const char *, node> files;

void ResourceManager::init()
{
}

void ResourceManager::deinit()
{
	for(auto ptr = files.begin(), end = files.end(); ptr != end; ++ptr) {
		if(ptr->second.refcount == 0)
			printf("Prune Node : %s\n", ptr->first);
		else
		{
			printf("Delete Node : %s\n", ptr->first);
			delete ptr->second.arc;
		}
	}
	files.clear();
}

Archive* ResourceManager::require(const char *path)
{
	auto entry = files.find(path);
	if(entry == files.end())
	{
		printf("New : %s\n", path);
		return files.insert({path, {new Archive(path), 1}}).first->second.arc;
		// return nullptr;
	}
	if(entry->second.refcount == 0)
	{
		printf("Reload : %s\n", path);
		entry->second.arc = new Archive(path);		
	}
	entry->second.refcount++;
	return entry->second.arc;
}

void ResourceManager::release(WalrusRPG::PIAF::Archive* arcs)
{
	for(auto ptr = files.begin(), end = files.end(); ptr != end; ++ptr)
	{
		if(ptr->second.arc == arcs)
		{
			ptr->second.refcount--;
			if(ptr->second.refcount == 0)
			{
				printf("Free : %s\n", ptr->first);
				delete ptr->second.arc;
				ptr->second.refcount = 0;
			}
			return;
		}
	}
}

void ResourceManager::release(const char *path)
{
	auto ptr = files.find(path);
	if(ptr == files.end()) return;
	ptr->second.refcount--;
	if(ptr->second.refcount == 0)
	{
		printf("Free : %s\n", ptr->first);
		delete ptr->second.arc;
		ptr->second.refcount = 0;
	}
	return;
}
