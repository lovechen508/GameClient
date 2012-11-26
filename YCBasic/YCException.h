#ifndef _INCLUDE_YCEXCEPTION_
#define _INCLUDE_YCEXCEPTION_

#include <exception>
#include <string>


class YCAPI YCException
	: public std::exception
{
public:
	YCException(int err, const char *desc);
	YCException(int err, const char *desc1, const char *desc2);
	YCException(int err, const char *desc, unsigned int code);

	virtual ~YCException() throw();
	virtual const char* what() const throw();
	int code() const {return err;}

private:
	char* description;
	const int err;
};
#endif