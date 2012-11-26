#ifndef _INCLUDE_YCCHATPARSER_H_
#define _INCLUDE_YCCHATPARSER_H_

struct NET_CHAT_MSG_REQ_T;
struct NET_CHAT_MSG_NTF_T;

class YCChatMessage;
/*
 * YCChatParser : ������Ϣ������
 */
struct YCChatParser
{
	//
	// ������build(const YCChatMessage* msg)
	//
	// Ŀ�ģ����ڽ���Ϣ��Ԫ�����������ݰ�
	//
	static NET_CHAT_MSG_REQ_T* build(const YCChatMessage* msg);

	//
	// ������parse(const NET_CHAT_MSG_NTF_T* msg)
	//
	// Ŀ�ģ����������ݰ������ڽ���Ϣ��Ԫ
	//
	static YCChatMessage* parse(const NET_CHAT_MSG_NTF_T* msg);
};

#endif

