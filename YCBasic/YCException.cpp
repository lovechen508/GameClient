#include "YCException.h"

#include "YCDef.h"

#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "YCAssert.h"

YCException::YCException(int err, const char *desc)
: err(err), description(NULL)
{
	SMART_ASSERT(desc != NULL);

	if (desc != NULL)
	{
		int length = strlen(desc);
		description = new char[length+1];
		memcpy(description, desc, length);
		description[length] = '\0';
	}
}

YCException::YCException(int err, const char *desc1, const char *desc2)
: err(err), description(NULL)
{
	SMART_ASSERT(desc1 != NULL);
	SMART_ASSERT(desc2 != NULL);
	if (desc1 != NULL && desc2 != NULL)
	{
		std::string desc(desc1);
		desc += ", ";
		desc += desc2;
		description = new char[desc.length()+1];
		memcpy(description, desc.c_str(), desc.length());
		description[desc.length()] = '\0';
	}
}

YCException::YCException(int err, const char *desc, unsigned int code)
	: err(err), description(NULL)
{
	SMART_ASSERT(desc != NULL);
	if (desc != NULL)
	{
		char desc2[32] = {0};
		itoa(code, desc2, 10);
		std::string strDesc(desc);
		strDesc += ", ";
		strDesc += desc2;
		description = new char[strDesc.length()+1];
		memcpy(description, strDesc.c_str(), strDesc.length());
		description[strDesc.length()] = '\0';
	}
}

YCException::~YCException() throw()
{
	SAFE_DELETE_ARRAY(description);
}

const char* YCException::what() const throw()
{
    return description;
}
