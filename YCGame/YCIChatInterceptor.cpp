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
// ������id()
//
// Ŀ�ģ�ȡ�ù�����Id
//
unsigned int YCIChatInterceptor::id() const
{
	return myId;
}

//
// ������setNext(YCIChatInterceptor* next)
//
// Ŀ�ģ��������ú�̹����������ؾ�
//
YCIChatInterceptor* YCIChatInterceptor::setNext(YCIChatInterceptor* next)
{
	YCIChatInterceptor* temp = myNext;
	myNext = next;
	return temp;
}

//
// ������setNext(YCIChatInterceptor* next)
//
// Ŀ�ģ���ȡ���ú�̹�����
//
YCIChatInterceptor* YCIChatInterceptor::getNext()
{
	return myNext;
}

//
// ������intercept(const YCChatMessage* msg)
//
// Ŀ�ģ������Ϣ�Ƿ�����, �����ط���true
//
bool YCIChatInterceptor::intercept(const YCChatMessage* msg)
{
	if (msg == NULL)
	{
		throw YCException(2002, "YCIChatInterceptor::validate��Ϣ��Ϊ��");
	}

	if (selfIntercept(msg))
	{
		return true;
	}

	return ((myNext == NULL) ? false : myNext->intercept(msg));
}
