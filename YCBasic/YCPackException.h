#ifndef _INCLUDE_YCPACKEXCEPTION_H_
#define _INCLUDE_YCPACKEXCEPTION_H_

#include "YCException.h"

/*
 * YCPackException : Pack�ļ������쳣
 */
class YCAPI YCPackException : public YCException
{
public:

	YCPackException(int err, const char *desc);
	YCPackException(int err, const char *desc1, const char *desc2);
	YCPackException(int err, const char *desc, unsigned int code);
	
	virtual ~YCPackException(void);
};

#endif

