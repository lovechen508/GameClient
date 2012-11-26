#ifndef _INCLUDE_YCINETWORKCALLBACK_H_
#define _INCLUDE_YCINETWORKCALLBACK_H_

#include "YCMSGDefine.h"

/*
 * YCINetworkCallback : 网络回调监听
 *
 */
class YCINetworkCallback
{
public:

	YCINetworkCallback(DECODE decoder) 
		: myDecoder(decoder)
	{
	}

	virtual ~YCINetworkCallback() {}

	//
	// 函数：handle(char* data, unsigned int len)
	//
	// 目的：处理网络包
	//
	virtual void handle(char* data, unsigned int len) = 0;

protected:
	
	DECODE myDecoder;

};

#endif