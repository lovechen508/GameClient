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
// 函数：selfValidate(const NET_CHAT_MSG_NTF_T* msg)
//
// 目的：自身校验消息是否合法
//
bool YCChannelFilter::selfValidate(const NET_CHAT_MSG_NTF_T* msg)
{/*
	// 发自自身
	if (msg->self == 1)
	{
		return true;
	}

	// 系统和彩世不能屏蔽
	if (msg->type == CH_SYSTEM || msg->type == CH_GLOBAL)
	{
		return true;
	}

	return ((msg->type & *myRefValidChannels) != 0);
*/
    return true;
}
