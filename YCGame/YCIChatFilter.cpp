#include "YCIChatFilter.h"

#include "YCChatMsgDefine.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

#include "YCNetwork\YCPkg_0080_Chat.h"

YCIChatFilter::YCIChatFilter(unsigned int id, YCIChatFilter* next)
	: myNext(next)
	, myId(id)
{
	SMART_ASSERT(id > 0);
}

YCIChatFilter::~YCIChatFilter(void)
{
	SAFE_DELETE(myNext);
}

//
// 函数：id()
//
// 目的：取得过滤器Id
//
unsigned int YCIChatFilter::id() const
{
	return myId;
}

//
// 函数：setNext(YCIChatFilter* next)
//
// 目的：重新设置后继过滤器，返回旧
//
YCIChatFilter* YCIChatFilter::setNext(YCIChatFilter* next)
{
	YCIChatFilter* temp = myNext;
	myNext = next;
	return temp;
}

//
// 函数：setNext(YCIChatFilter* next)
//
// 目的：获取设置后继过滤器
//
YCIChatFilter* YCIChatFilter::getNext()
{
	return myNext;
}

//
// 函数：validate(const NET_CHAT_MSG_NTF_T* msg)
//
// 目的：校验消息是否合法，合法返回true
//
bool YCIChatFilter::validate(const NET_CHAT_MSG_NTF_T* msg)
{
	if (msg == NULL)
	{
		throw YCException(2002, "YCIChatFilter::validate消息体为空");
	}

	if (selfValidate(msg))
	{
		return ((myNext == NULL) ? true : myNext->validate(msg));
	}
	return false;
}
