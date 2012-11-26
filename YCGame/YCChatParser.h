#ifndef _INCLUDE_YCCHATPARSER_H_
#define _INCLUDE_YCCHATPARSER_H_

struct NET_CHAT_MSG_REQ_T;
struct NET_CHAT_MSG_NTF_T;

class YCChatMessage;
/*
 * YCChatParser : 聊天消息解析类
 */
struct YCChatParser
{
	//
	// 函数：build(const YCChatMessage* msg)
	//
	// 目的：从内建消息单元构建网络数据包
	//
	static NET_CHAT_MSG_REQ_T* build(const YCChatMessage* msg);

	//
	// 函数：parse(const NET_CHAT_MSG_NTF_T* msg)
	//
	// 目的：从网络数据包构建内建消息单元
	//
	static YCChatMessage* parse(const NET_CHAT_MSG_NTF_T* msg);
};

#endif

