#include "YCChatParser.h"

#include "YCChatManager.h"
#include "YCChatMsgDefine.h"

#include "YCBasic\YCAssert.h"

#include "YCNetwork\YCPkg_0080_Chat.h"

//
// ������build(const YCChatMessage* msg)
//
// Ŀ�ģ����ڽ���Ϣ��Ԫ�����������ݰ�
//
NET_CHAT_MSG_REQ_T* YCChatParser::build(const YCChatMessage* msg)
{
	return NULL;
}

//
// ������parse(const NET_CHAT_MSG_NTF_T* msg)
//
// Ŀ�ģ�������Ϣ��Ԫ
//
YCChatMessage* YCChatParser::parse(const NET_CHAT_MSG_NTF_T* msg)
{
	return NULL;
}
