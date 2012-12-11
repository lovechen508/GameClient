#include "YCPkg_0090_Npc.h"

#include "..\YCNetwork.h"
#include "..\YCDataHolder.h"

int Encode_GSMSG_TALKNPC_REQ(void* host, YCDataHolder* holder)
{
	GSMSG_TALKNPC_REQ* pkg = (GSMSG_TALKNPC_REQ*)host;
	if (-1 == holder->addDword(pkg->dwObjectId)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_GSMSG_NPCSELECTION_REQ(void* host, YCDataHolder* holder)
{
	GSMSG_NPCSELECTION_REQ* pkg = (GSMSG_NPCSELECTION_REQ*)host;
	if (-1 == holder->addWord(pkg->wSelection)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_GSMSG_TALKNPC_NTF(void* host, YCDataHolder* holder)
{
	GSMSG_TALKNPC_NTF* pkg = (GSMSG_TALKNPC_NTF*)host;
	if (-1 == holder->addDword(pkg->dwObjectId)) {
		return -1;
	}
	if (-1 == holder->addWord(pkg->wLen)) {
		return -1;
	}
	if (-1 == holder->addString(pkg->szMessage, MAX_NPC_MSG_LEN)) {
		return -1;
	}
	return holder->getPosition();
}

int Decode_GSMSG_TALKNPC_REQ(void* host, YCDataHolder* holder)
{
	GSMSG_TALKNPC_REQ* pkg = (GSMSG_TALKNPC_REQ*)host;
	if (-1 == holder->delDword(pkg->dwObjectId)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_GSMSG_NPCSELECTION_REQ(void* host, YCDataHolder* holder)
{
	GSMSG_NPCSELECTION_REQ* pkg = (GSMSG_NPCSELECTION_REQ*)host;
	if (-1 == holder->delWord(pkg->wSelection)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_GSMSG_TALKNPC_NTF(void* host, YCDataHolder* holder)
{
	GSMSG_TALKNPC_NTF* pkg = (GSMSG_TALKNPC_NTF*)host;
	if (-1 == holder->delDword(pkg->dwObjectId)) {
		return -1;
	}
	if (-1 == holder->delWord(pkg->wLen)) {
		return -1;
	}
	if (-1 == holder->delString(pkg->szMessage, MAX_NPC_MSG_LEN)) {
		return -1;
	}
	return holder->getPosition();
}


/////////////////////////////////////////////////////////
// YCPkg_0090_Npc 初始化函数实现
/////////////////////////////////////////////////////////
void YCPkg_0090_Npc_init(YCNetwork* network)
{
	network->registry(81, Encode_GSMSG_TALKNPC_REQ, Decode_GSMSG_TALKNPC_REQ);
	network->registry(83, Encode_GSMSG_NPCSELECTION_REQ, Decode_GSMSG_NPCSELECTION_REQ);
	network->registry(82, Encode_GSMSG_TALKNPC_NTF, Decode_GSMSG_TALKNPC_NTF);
}

