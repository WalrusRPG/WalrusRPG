#include "Quirks.h"
#include <cstddef>
#include <cstring>
#include <memory>
#include "Logger.h"

using namespace WalrusRPG;

void Quirks::init(const char *argv_0)
{
  WalrusRPG::Logger::log("Quirks init");
}

void Quirks::deinit()
{
  WalrusRPG::Logger::log("Quirks deinit");
}

std::unique_ptr<char> Quirks::solve_absolute_path(const char *path)
{
  std::unique_ptr<char> result(new char[strlen(path) + 1]);
  strcpy(result.get(), path);
  return result;
}

#include <stdlib.h>
#include <unistd.h>  /* for write(), also available on Windows */
extern "C" void* emulate_cc_new(unsigned len) { \
  void *p = malloc(len);
  if (p == 0) {
    /* Don't use stdio (e.g. fputs), because that may want to allocate more
     * memory.
     */
    (void)!write(2, "out of memory\n", 14);
    abort();
  }
  return p;
}
extern "C" void emulate_cc_delete(void* p) {
  if (p != 0)
    free(p);
}
void* operator new  (unsigned len) __attribute__((alias("emulate_cc_new")));
void* operator new[](unsigned len) __attribute__((alias("emulate_cc_new")));
void  operator delete  (void* p)   __attribute__((alias("emulate_cc_delete")));
void  operator delete[](void* p)   __attribute__((alias("emulate_cc_delete")));

extern "C" void __cxa_pure_virtual()
{
  //while (1);
}
