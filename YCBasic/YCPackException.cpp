#include "YCPackException.h"


YCPackException::YCPackException(int err, const char *desc)
	: YCException(err, desc)
{

}

YCPackException::YCPackException(int err, const char *desc1, const char *desc2)
	: YCException(err, desc1, desc2)
{

}

YCPackException::YCPackException(int err, const char *desc, unsigned int code)
	: YCException(err, desc, code)
{

}


YCPackException::~YCPackException(void)
{
}
