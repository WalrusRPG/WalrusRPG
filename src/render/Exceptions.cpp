#include "Font.h"
#include "Logger.h"
#include <cstdarg>
#include <cstring>

using WalrusRPG::Graphics::FontException;
using namespace WalrusRPG::Logger;

FontException::FontException(const char *format, ...) : msg{0}
{
    va_list list;
    va_start(list, format);
    vsnprintf(msg, 1024, format, list);
    error(msg);
    va_end(list);
}

FontException::~FontException()
{
}

const char *FontException::what() const throw()
{
    return msg;
}