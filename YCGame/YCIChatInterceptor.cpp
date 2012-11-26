#include "YCIChatInterceptor.h"

#include "YCChatMsgDefine.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCIChatInterceptor::YCIChatInterceptor(unsigned int id, YCIChatInterceptor* next)
	: myNext(next)
	, myId(id)
{
	SMART_ASSERT(id > 0);
}

YCIChatInterceptor::~YCIChatInterceptor(void)
{
	SAFE_DELETE(myNext);
}

//
// 函数：id()
//
// 目的：取得过滤器Id
//
unsigned int YCIChatInterceptor::id() const
{
	return myId;
}

//
// 函数：setNext(YCIChatInterceptor* next)
//
// 目的：重新设置后继过滤器，返回旧
//
YCIChatInterceptor* YCIChatInterceptor::setNext(YCIChatInterceptor* next)
{
	YCIChatInterceptor* temp = myNext;
	myNext = next;
	return temp;
}

//
// 函数：setNext(YCIChatInterceptor* next)
//
// 目的：获取设置后继过滤器
//
YCIChatInterceptor* YCIChatInterceptor::getNext()
{
	return myNext;
}

//
// 函数：intercept(const YCChatMessage* msg)
//
// 目的：检查消息是否拦截, 被拦截返回true
//
bool YCIChatInterceptor::intercept(const YCChatMessage* msg)
{
	if (msg == NULL)
	{
		throw YCException(2002, "YCIChatInterceptor::validate消息体为空");
	}

	if (selfIntercept(msg))
	{
		return true;
	}

	return ((myNext == NULL) ? false : myNext->intercept(msg));
}
