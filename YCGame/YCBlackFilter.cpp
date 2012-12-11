#include "YCBlackFilter.h"

#include "YCChatMsgDefine.h"

#include "YCBasic\YCDList.h"

#include "YCNetwork\protocol\YCPkg_0080_Chat.h"

YCBlackFilter::YCBlackFilter(const YCDList* blacklist, YCIChatFilter* next)
	: YCIChatFilter(BLACK_FILTER_ID, next)
	, myRefBlacklist(blacklist)
{
}

YCBlackFilter::~YCBlackFilter(void)
{
}

//
// 函数：selfValidate(const NET_CHAT_MSG_NTF_T* msg)
//
// 目的：自身校验消息是否合法
//
bool YCBlackFilter::selfValidate(const NET_CHAT_MSG_NTF_T* msg)
{
	// 发自自身
    /*
	if (msg->self == 1)
	{
		return true;
	}

	for (const Item_List_T* item = myRefBlacklist->begin();
		 item != myRefBlacklist->end();
		 item = myRefBlacklist->next(item))
	{
		const char* black = (const char*)myRefBlacklist->payload(item);
		if (stricmp(black, msg->role) == 0)
		{
			return false;
		}
	}
    */
	return true;
}
