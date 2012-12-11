#include "YCPkg_1002_LeaveScene.h"

#include "..\YCNetwork.h"
#include "..\YCDataHolder.h"

int Encode_GSMSG_LEAVEGAME_REQ(void* host, YCDataHolder* holder)
{
	GSMSG_LEAVEGAME_REQ* pkg = (GSMSG_LEAVEGAME_REQ*)host;
	if (-1 == holder->addByte(pkg->byType)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_GSMSG_LEAVEGAME_NTF(void* host, YCDataHolder* holder)
{
	GSMSG_LEAVEGAME_NTF* pkg = (GSMSG_LEAVEGAME_NTF*)host;
	if (-1 == holder->addInt(pkg->nErrCode)) {
		return -1;
	}
	if (-1 == holder->addByte(pkg->byType)) {
		return -1;
	}
	return holder->getPosition();
}

int Decode_GSMSG_LEAVEGAME_REQ(void* host, YCDataHolder* holder)
{
	GSMSG_LEAVEGAME_REQ* pkg = (GSMSG_LEAVEGAME_REQ*)host;
	if (-1 == holder->delByte(pkg->byType)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_GSMSG_LEAVEGAME_NTF(void* host, YCDataHolder* holder)
{
	GSMSG_LEAVEGAME_NTF* pkg = (GSMSG_LEAVEGAME_NTF*)host;
	if (-1 == holder->delInt(pkg->nErrCode)) {
		return -1;
	}
	if (-1 == holder->delByte(pkg->byType)) {
		return -1;
	}
	return holder->getPosition();
}


/////////////////////////////////////////////////////////
// YCPkg_1002_LeaveScene 初始化函数实现
/////////////////////////////////////////////////////////
void YCPkg_1002_LeaveScene_init(YCNetwork* network)
{
	network->registry(1002, Encode_GSMSG_LEAVEGAME_REQ, Decode_GSMSG_LEAVEGAME_REQ);
	network->registry(1003, Encode_GSMSG_LEAVEGAME_NTF, Decode_GSMSG_LEAVEGAME_NTF);
}

