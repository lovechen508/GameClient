#ifndef _INCLUDE_YCNETWORKCALLBACKHELPER_H_
#define _INCLUDE_YCNETWORKCALLBACKHELPER_H_

#include "YCINetworkCallback.h"
#include "YCDataHolder.h"

template<typename T, typename P>
class YCDecodeHelper : public YCINetworkCallback
{
	typedef typename T Delegate;
	typedef typename P Package;

public:

	YCDecodeHelper(Delegate* host, DECODE decoder)
		: YCINetworkCallback(decoder)
		, myDelegate(host)
		
	{
	}

	~YCDecodeHelper() {}

	void handle(char* data, unsigned int len)
	{
		Package pkg;
		YCDataHolder holder(data, len);

		if (myDecoder(&pkg, &holder))
		{
			myDelegate->handle(pkg);
		}
	}

private:

	Delegate* myDelegate;
};

#endif