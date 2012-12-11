#include "YCPkg_1012_Move.h"

#include "..\YCNetwork.h"
#include "..\YCDataHolder.h"

int Encode_GSPKG_MOVE_REQ(void* host, YCDataHolder* holder)
{
	GSPKG_MOVE_REQ* pkg = (GSPKG_MOVE_REQ*)host;
	if (-1 == holder->addByte(pkg->byType)) {
		return -1;
	}
	if (-1 == holder->addByte(pkg->byDirection)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_GSPKG_MOVE_ACK(void* host, YCDataHolder* holder)
{
	GSPKG_MOVE_ACK* pkg = (GSPKG_MOVE_ACK*)host;
	if (-1 == holder->addInt(pkg->nErrCode)) {
		return -1;
	}
	if (-1 == holder->addByte(pkg->byType)) {
		return -1;
	}
	if (-1 == holder->addWord(pkg->x)) {
		return -1;
	}
	if (-1 == holder->addWord(pkg->y)) {
		return -1;
	}
	return holder->getPosition();
}

int Decode_GSPKG_MOVE_REQ(void* host, YCDataHolder* holder)
{
	GSPKG_MOVE_REQ* pkg = (GSPKG_MOVE_REQ*)host;
	if (-1 == holder->delByte(pkg->byType)) {
		return -1;
	}
	if (-1 == holder->delByte(pkg->byDirection)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_GSPKG_MOVE_ACK(void* host, YCDataHolder* holder)
{
	GSPKG_MOVE_ACK* pkg = (GSPKG_MOVE_ACK*)host;
	if (-1 == holder->delInt(pkg->nErrCode)) {
		return -1;
	}
	if (-1 == holder->delByte(pkg->byType)) {
		return -1;
	}
	if (-1 == holder->delWord(pkg->x)) {
		return -1;
	}
	if (-1 == holder->delWord(pkg->y)) {
		return -1;
	}
	return holder->getPosition();
}


/////////////////////////////////////////////////////////
// YCPkg_1012_Move 初始化函数实现
/////////////////////////////////////////////////////////
void YCPkg_1012_Move_init(YCNetwork* network)
{
	network->registry(1012, Encode_GSPKG_MOVE_REQ, Decode_GSPKG_MOVE_REQ);
	network->registry(1013, Encode_GSPKG_MOVE_ACK, Decode_GSPKG_MOVE_ACK);
}

