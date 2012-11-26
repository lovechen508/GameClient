#ifndef _INCLUDE_YCBLACKFILTER_H_
#define _INCLUDE_YCBLACKFILTER_H_

#include "YCIChatFilter.h"

class YCDList;

/*
 * YCBlackFilter : 黑名单过滤器
 */
class YCBlackFilter :
	public YCIChatFilter
{
public:

	YCBlackFilter(const YCDList* blacklist, YCIChatFilter* next);

	virtual ~YCBlackFilter(void);

private:

	//
	// 函数：selfValidate(const NET_CHAT_MSG_NTF_T* msg)
	//
	// 目的：自身校验消息是否合法
	//
	virtual bool selfValidate(const NET_CHAT_MSG_NTF_T* msg);

private:

	//
	// 黑名单列表，引用, 不持有生命周期
	//
	const YCDList* myRefBlacklist;
};

#endif

