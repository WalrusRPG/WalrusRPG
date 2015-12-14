#include "Archive.h"
#include <cmath>
#include <cstring>

using WalrusRPG::PIAF::PIAFException;
using namespace WalrusRPG::PIAF;

PIAFException::PIAFException(const char *file, const unsigned line, const char *message)
	: msg(nullptr)
{
	unsigned length_of_digit = ((line != 0) ? log10(line) + 1 : 1);
	unsigned length_of_string = strlen(file) + 3 + length_of_digit + 3 + strlen(message);

	msg = new char[length_of_string + 1];
	if(msg != nullptr)
	{
		snprintf(msg, length_of_string+1, "%s - %d : %s", file, line, message);
		msg[length_of_string] = '\0';		
	}
}

PIAFException::~PIAFException()
{
	if(msg != nullptr)
		delete msg;
}

const char* PIAFException::what() const throw()
{
	return msg;
}