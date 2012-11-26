#ifndef _INCLUDE_YCINETWORKCALLBACK_H_
#define _INCLUDE_YCINETWORKCALLBACK_H_

#include "YCMSGDefine.h"

/*
 * YCINetworkCallback : ����ص�����
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
	// ������handle(char* data, unsigned int len)
	//
	// Ŀ�ģ����������
	//
	virtual void handle(char* data, unsigned int len) = 0;

protected:
	
	DECODE myDecoder;

};

#endif