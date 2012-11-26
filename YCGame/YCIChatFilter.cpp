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
// ������id()
//
// Ŀ�ģ�ȡ�ù�����Id
//
unsigned int YCIChatFilter::id() const
{
	return myId;
}

//
// ������setNext(YCIChatFilter* next)
//
// Ŀ�ģ��������ú�̹����������ؾ�
//
YCIChatFilter* YCIChatFilter::setNext(YCIChatFilter* next)
{
	YCIChatFilter* temp = myNext;
	myNext = next;
	return temp;
}

//
// ������setNext(YCIChatFilter* next)
//
// Ŀ�ģ���ȡ���ú�̹�����
//
YCIChatFilter* YCIChatFilter::getNext()
{
	return myNext;
}

//
// ������validate(const NET_CHAT_MSG_NTF_T* msg)
//
// Ŀ�ģ�У����Ϣ�Ƿ�Ϸ����Ϸ�����true
//
bool YCIChatFilter::validate(const NET_CHAT_MSG_NTF_T* msg)
{
	if (msg == NULL)
	{
		throw YCException(2002, "YCIChatFilter::validate��Ϣ��Ϊ��");
	}

	if (selfValidate(msg))
	{
		return ((myNext == NULL) ? true : myNext->validate(msg));
	}
	return false;
}
