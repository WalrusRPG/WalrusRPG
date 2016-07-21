#include "Logger.h"
#include "Map.h"
#include "Tileset.h"
#include <cstdarg>
#include <cstring>

using WalrusRPG::MapException;
using WalrusRPG::TilesetException;
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

TilesetException::TilesetException(const char *format, ...) : msg("")
{
    va_list list;
    va_start(list, format);
    vsnprintf(msg, 1024, format, list);
    error(msg);
    va_end(list);
}

TilesetException::~TilesetException()
{
}

const char *TilesetException::what() const throw()
{
    return msg;
}
