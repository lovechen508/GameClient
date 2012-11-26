#include "YCChatParser.h"

#include "YCChatManager.h"
#include "YCChatMsgDefine.h"

#include "YCBasic\YCAssert.h"

#include "YCNetwork\YCPkg_0080_Chat.h"

//
// 函数：build(const YCChatMessage* msg)
//
// 目的：从内建消息单元构建网络数据包
//
NET_CHAT_MSG_REQ_T* YCChatParser::build(const YCChatMessage* msg)
{
	return NULL;
}

//
// 函数：parse(const NET_CHAT_MSG_NTF_T* msg)
//
// 目的：构建消息单元
//
YCChatMessage* YCChatParser::parse(const NET_CHAT_MSG_NTF_T* msg)
{
	return NULL;
}
