#include "YCPkg_1000_EnterScene.h"

#include "..\YCNetwork.h"
#include "..\YCDataHolder.h"

int Encode_GSMSG_ENTERGAME_REQ(void* host, YCDataHolder* holder)
{
	GSMSG_ENTERGAME_REQ* pkg = (GSMSG_ENTERGAME_REQ*)host;
	if (-1 == holder->addString(pkg->szRolename, ROLE_NAME_LEN)) {
		return -1;
	}
	if (-1 == holder->addUint64(pkg->sessionId)) {
		return -1;
	}
	if (-1 == holder->addDword(pkg->dwFlags)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_GSMSG_ENTERGAME_NTF(void* host, YCDataHolder* holder)
{
	GSMSG_ENTERGAME_NTF* pkg = (GSMSG_ENTERGAME_NTF*)host;
	if (-1 == holder->addInt(pkg->nErrCode)) {
		return -1;
	}
	if (-1 == holder->addDword(pkg->dwFlags)) {
		return -1;
	}
	return holder->getPosition();
}

int Decode_GSMSG_ENTERGAME_REQ(void* host, YCDataHolder* holder)
{
	GSMSG_ENTERGAME_REQ* pkg = (GSMSG_ENTERGAME_REQ*)host;
	if (-1 == holder->delString(pkg->szRolename, ROLE_NAME_LEN)) {
		return -1;
	}
	if (-1 == holder->delUint64(pkg->sessionId)) {
		return -1;
	}
	if (-1 == holder->delDword(pkg->dwFlags)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_GSMSG_ENTERGAME_NTF(void* host, YCDataHolder* holder)
{
	GSMSG_ENTERGAME_NTF* pkg = (GSMSG_ENTERGAME_NTF*)host;
	if (-1 == holder->delInt(pkg->nErrCode)) {
		return -1;
	}
	if (-1 == holder->delDword(pkg->dwFlags)) {
		return -1;
	}
	return holder->getPosition();
}


/////////////////////////////////////////////////////////
// YCPkg_1000_EnterScene 初始化函数实现
/////////////////////////////////////////////////////////
void YCPkg_1000_EnterScene_init(YCNetwork* network)
{
	network->registry(1000, Encode_GSMSG_ENTERGAME_REQ, Decode_GSMSG_ENTERGAME_REQ);
	network->registry(1001, Encode_GSMSG_ENTERGAME_NTF, Decode_GSMSG_ENTERGAME_NTF);
}

