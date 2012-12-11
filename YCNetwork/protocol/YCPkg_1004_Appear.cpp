#include "YCPkg_1004_Appear.h"

#include "..\YCNetwork.h"
#include "..\YCDataHolder.h"

int Encode_ROLE_BASIC_INFO(void* host, YCDataHolder* holder)
{
	ROLE_BASIC_INFO* pkg = (ROLE_BASIC_INFO*)host;
	if (-1 == holder->addString(pkg->roleName, ROLE_NAME_LEN)) {
		return -1;
	}
	if (-1 == holder->addDword(pkg->objectId)) {
		return -1;
	}
	if (-1 == holder->addWord(pkg->level)) {
		return -1;
	}
	if (-1 == holder->addByte(pkg->direction)) {
		return -1;
	}
	if (-1 == holder->addWord(pkg->x)) {
		return -1;
	}
	if (-1 == holder->addWord(pkg->y)) {
		return -1;
	}
	if (-1 == holder->addByte(pkg->career)) {
		return -1;
	}
	if (-1 == holder->addByte(pkg->sex)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_ROLE_ADDITION_INFO(void* host, YCDataHolder* holder)
{
	ROLE_ADDITION_INFO* pkg = (ROLE_ADDITION_INFO*)host;
	if (-1 == holder->addByte(pkg->portrait)) {
		return -1;
	}
	if (-1 == holder->addByte(pkg->hairType)) {
		return -1;
	}
	if (-1 == holder->addWord(pkg->helmId)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_GSPKG_ROLEMAINDATA_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_ROLEMAINDATA_NTF* pkg = (GSPKG_ROLEMAINDATA_NTF*)host;
	if (-1 == Encode_ROLE_BASIC_INFO(&pkg->basic, holder)) {
		return -1;
	}
	if (-1 == Encode_ROLE_ADDITION_INFO(&pkg->addition, holder)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_GSPKG_ROLEPOSITION_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_ROLEPOSITION_NTF* pkg = (GSPKG_ROLEPOSITION_NTF*)host;
	if (-1 == holder->addWord(pkg->wMapCode)) {
		return -1;
	}
	if (-1 == holder->addString(pkg->szMapName, GS_MAPNAME_LEN)) {
		return -1;
	}
	if (-1 == holder->addWord(pkg->x)) {
		return -1;
	}
	if (-1 == holder->addWord(pkg->y)) {
		return -1;
	}
	if (-1 == holder->addByte(pkg->byDirection)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_GSPKG_ROLEAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_ROLEAPPEAR_NTF* pkg = (GSPKG_ROLEAPPEAR_NTF*)host;
	if (-1 == Encode_ROLE_BASIC_INFO(&pkg->basic, holder)) {
		return -1;
	}
	if (-1 == Encode_ROLE_ADDITION_INFO(&pkg->addition, holder)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_GSPKG_MONSTERAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_MONSTERAPPEAR_NTF* pkg = (GSPKG_MONSTERAPPEAR_NTF*)host;
	return holder->getPosition();
}
int Encode_GSPKG_NPCAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_NPCAPPEAR_NTF* pkg = (GSPKG_NPCAPPEAR_NTF*)host;
	return holder->getPosition();
}
int Encode_GSPKG_ITEMAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_ITEMAPPEAR_NTF* pkg = (GSPKG_ITEMAPPEAR_NTF*)host;
	return holder->getPosition();
}
int Encode_GSPKG_MAGICAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_MAGICAPPEAR_NTF* pkg = (GSPKG_MAGICAPPEAR_NTF*)host;
	return holder->getPosition();
}
int Encode_GSPKG_OBJECTDISAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_OBJECTDISAPPEAR_NTF* pkg = (GSPKG_OBJECTDISAPPEAR_NTF*)host;
	return holder->getPosition();
}

int Decode_ROLE_BASIC_INFO(void* host, YCDataHolder* holder)
{
	ROLE_BASIC_INFO* pkg = (ROLE_BASIC_INFO*)host;
	if (-1 == holder->delString(pkg->roleName, ROLE_NAME_LEN)) {
		return -1;
	}
	if (-1 == holder->delDword(pkg->objectId)) {
		return -1;
	}
	if (-1 == holder->delWord(pkg->level)) {
		return -1;
	}
	if (-1 == holder->delByte(pkg->direction)) {
		return -1;
	}
	if (-1 == holder->delWord(pkg->x)) {
		return -1;
	}
	if (-1 == holder->delWord(pkg->y)) {
		return -1;
	}
	if (-1 == holder->delByte(pkg->career)) {
		return -1;
	}
	if (-1 == holder->delByte(pkg->sex)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_ROLE_ADDITION_INFO(void* host, YCDataHolder* holder)
{
	ROLE_ADDITION_INFO* pkg = (ROLE_ADDITION_INFO*)host;
	if (-1 == holder->delByte(pkg->portrait)) {
		return -1;
	}
	if (-1 == holder->delByte(pkg->hairType)) {
		return -1;
	}
	if (-1 == holder->delWord(pkg->helmId)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_GSPKG_ROLEMAINDATA_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_ROLEMAINDATA_NTF* pkg = (GSPKG_ROLEMAINDATA_NTF*)host;
	if (-1 == Encode_ROLE_BASIC_INFO(&pkg->basic, holder)) {
		return -1;
	}
	if (-1 == Encode_ROLE_ADDITION_INFO(&pkg->addition, holder)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_GSPKG_ROLEPOSITION_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_ROLEPOSITION_NTF* pkg = (GSPKG_ROLEPOSITION_NTF*)host;
	if (-1 == holder->delWord(pkg->wMapCode)) {
		return -1;
	}
	if (-1 == holder->delString(pkg->szMapName, GS_MAPNAME_LEN)) {
		return -1;
	}
	if (-1 == holder->delWord(pkg->x)) {
		return -1;
	}
	if (-1 == holder->delWord(pkg->y)) {
		return -1;
	}
	if (-1 == holder->delByte(pkg->byDirection)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_GSPKG_ROLEAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_ROLEAPPEAR_NTF* pkg = (GSPKG_ROLEAPPEAR_NTF*)host;
	if (-1 == Encode_ROLE_BASIC_INFO(&pkg->basic, holder)) {
		return -1;
	}
	if (-1 == Encode_ROLE_ADDITION_INFO(&pkg->addition, holder)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_GSPKG_MONSTERAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_MONSTERAPPEAR_NTF* pkg = (GSPKG_MONSTERAPPEAR_NTF*)host;
	return holder->getPosition();
}
int Decode_GSPKG_NPCAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_NPCAPPEAR_NTF* pkg = (GSPKG_NPCAPPEAR_NTF*)host;
	return holder->getPosition();
}
int Decode_GSPKG_ITEMAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_ITEMAPPEAR_NTF* pkg = (GSPKG_ITEMAPPEAR_NTF*)host;
	return holder->getPosition();
}
int Decode_GSPKG_MAGICAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_MAGICAPPEAR_NTF* pkg = (GSPKG_MAGICAPPEAR_NTF*)host;
	return holder->getPosition();
}
int Decode_GSPKG_OBJECTDISAPPEAR_NTF(void* host, YCDataHolder* holder)
{
	GSPKG_OBJECTDISAPPEAR_NTF* pkg = (GSPKG_OBJECTDISAPPEAR_NTF*)host;
	return holder->getPosition();
}


/////////////////////////////////////////////////////////
// YCPkg_1004_Appear 初始化函数实现
/////////////////////////////////////////////////////////
void YCPkg_1004_Appear_init(YCNetwork* network)
{
	network->registry(1004, Encode_GSPKG_ROLEMAINDATA_NTF, Decode_GSPKG_ROLEMAINDATA_NTF);
	network->registry(1005, Encode_GSPKG_ROLEPOSITION_NTF, Decode_GSPKG_ROLEPOSITION_NTF);
	network->registry(1006, Encode_GSPKG_ROLEAPPEAR_NTF, Decode_GSPKG_ROLEAPPEAR_NTF);
	network->registry(1007, Encode_GSPKG_MONSTERAPPEAR_NTF, Decode_GSPKG_MONSTERAPPEAR_NTF);
	network->registry(1008, Encode_GSPKG_NPCAPPEAR_NTF, Decode_GSPKG_NPCAPPEAR_NTF);
	network->registry(1009, Encode_GSPKG_ITEMAPPEAR_NTF, Decode_GSPKG_ITEMAPPEAR_NTF);
	network->registry(1010, Encode_GSPKG_MAGICAPPEAR_NTF, Decode_GSPKG_MAGICAPPEAR_NTF);
	network->registry(1011, Encode_GSPKG_OBJECTDISAPPEAR_NTF, Decode_GSPKG_OBJECTDISAPPEAR_NTF);
}

