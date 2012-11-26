#ifndef _INCLUDE_YCIPHELPER_H_
#define _INCLUDE_YCIPHELPER_H_

#include <winSock2.h>

struct YCIPHelper
{
	static bool isIPAddress(const char* ip)
	{
		char c;
		size_t len = strlen(ip);
		for (size_t i = 0; i < len; ++i)
		{
			c = ip[i];
			if (c == '.')
				continue;
			if (c < '0' || c > '9')
				return false;
		}
		return true;
	}

	static u_long getIPAddress(const char* ip)
	{
		u_long addr = 0x0;
		if (isIPAddress(ip))
			addr = inet_addr(ip);
		else
		{
			hostent* host = gethostbyname(ip);
			if (host && host->h_addr)
				addr = *(u_long*)host->h_addr;
		}
		return addr;
	}
};

#endif