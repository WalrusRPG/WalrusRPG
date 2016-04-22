#include "Logger.h"
#include "Map.h"
#include <cstring>
#include <cstdarg>

using WalrusRPG::MapException;
using namespace WalrusRPG::Logger;

MapException::MapException(const char *format, ...) : msg("")
{
    va_list list;
    va_start(list, format);
    vsnprintf(msg, 1024, format, list);
    error(msg);
    va_end(list);
}

MapException::~MapException()
{
}

const char *MapException::what() const throw()
{
    return msg;
}
