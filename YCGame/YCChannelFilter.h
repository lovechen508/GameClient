#ifndef _INCLUDE_YCCHANNELFILTER_H_
#define _INCLUDE_YCCHANNELFILTER_H_

#include "YCIChatFilter.h"

/*
 * YCChannelFilter : Ƶ��������
 */
class YCChannelFilter :
	public YCIChatFilter
{
public:

	YCChannelFilter(const int* validChannels, YCIChatFilter* next);

	virtual ~YCChannelFilter(void);

private:

	//
	// ������selfValidate(const NET_CHAT_MSG_NTF_T* msg)
	//
	// Ŀ�ģ�����У����Ϣ�Ƿ�Ϸ�
	//
	virtual bool selfValidate(const NET_CHAT_MSG_NTF_T* msg);

private:

	//
	// ��ЧƵ��������, ��������������
	//
	const int* myRefValidChannels;
};

#endif

