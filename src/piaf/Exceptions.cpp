#include "Logger.h"
#include "Archive.h"
#include <cmath>
#include <cstring>
#include <cstdarg>

using WalrusRPG::PIAF::PIAFException;
using namespace WalrusRPG::PIAF;
using namespace WalrusRPG::Logger;

PIAFException::PIAFException(const char *format, ...) : msg{0}
{
    va_list list;
    va_start(list, format);
    vsnprintf(msg, 1024, format, list);
    va_end(list);
}

PIAFException::~PIAFException()
{
}

const char *PIAFException::what() const throw()
{
    return msg;
}
