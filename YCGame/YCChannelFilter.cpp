#include "YCChannelFilter.h"

#include "YCChatMsgDefine.h"

#include "YCNetwork\protocol\YCPkg_0080_Chat.h"

YCChannelFilter::YCChannelFilter(const int* validChannels, YCIChatFilter* next)
	: YCIChatFilter(CH_FILTER_ID, next)
	, myRefValidChannels(validChannels)
{
}

YCChannelFilter::~YCChannelFilter(void)
{
}

//
// ������selfValidate(const NET_CHAT_MSG_NTF_T* msg)
//
// Ŀ�ģ�����У����Ϣ�Ƿ�Ϸ�
//
bool YCChannelFilter::selfValidate(const NET_CHAT_MSG_NTF_T* msg)
{/*
	// ��������
	if (msg->self == 1)
	{
		return true;
	}

	// ϵͳ�Ͳ�����������
	if (msg->type == CH_SYSTEM || msg->type == CH_GLOBAL)
	{
		return true;
	}

	return ((msg->type & *myRefValidChannels) != 0);
*/
    return true;
}
