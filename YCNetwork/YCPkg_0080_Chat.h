#ifndef _INCLUDE_YCPKG_0080_CHAT_H_
#define _INCLUDE_YCPKG_0080_CHAT_H_

/*
 * ������Ϣ����ṹ��
 */
struct NET_CHAT_MSG_REQ_T
{
	unsigned char	type;
	unsigned char	self;
	char			role[ROLE_NAME_LENGTH];
};

/*
 * ������Ϣ��Ӧ�ṹ��
 */
struct NET_CHAT_MSG_NTF_T
{
	unsigned char	type;
	unsigned char	self;
	char			role[ROLE_NAME_LENGTH];
};

#endif