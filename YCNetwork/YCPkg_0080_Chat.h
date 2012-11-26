#ifndef _INCLUDE_YCPKG_0080_CHAT_H_
#define _INCLUDE_YCPKG_0080_CHAT_H_

/*
 * 聊天消息请求结构体
 */
struct NET_CHAT_MSG_REQ_T
{
	unsigned char	type;
	unsigned char	self;
	char			role[ROLE_NAME_LENGTH];
};

/*
 * 聊天消息响应结构体
 */
struct NET_CHAT_MSG_NTF_T
{
	unsigned char	type;
	unsigned char	self;
	char			role[ROLE_NAME_LENGTH];
};

#endif