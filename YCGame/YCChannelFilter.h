#ifndef _INCLUDE_YCCHANNELFILTER_H_
#define _INCLUDE_YCCHANNELFILTER_H_

#include "YCIChatFilter.h"

/*
 * YCChannelFilter : 频道过滤器
 */
class YCChannelFilter :
	public YCIChatFilter
{
public:

	YCChannelFilter(const int* validChannels, YCIChatFilter* next);

	virtual ~YCChannelFilter(void);

private:

	//
	// 函数：selfValidate(const NET_CHAT_MSG_NTF_T* msg)
	//
	// 目的：自身校验消息是否合法
	//
	virtual bool selfValidate(const NET_CHAT_MSG_NTF_T* msg);

private:

	//
	// 有效频道，引用, 不持有生命周期
	//
	const int* myRefValidChannels;
};

#endif

