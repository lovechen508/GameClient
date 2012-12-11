#include "YCPkg_0001_Version.h"

#include "..\YCNetwork.h"
#include "..\YCDataHolder.h"

int Encode_GGPKG_VERSION_RPT(void* host, YCDataHolder* holder)
{
	GGPKG_VERSION_RPT* pkg = (GGPKG_VERSION_RPT*)host;
	if (-1 == holder->addDword(pkg->version)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_GGPKG_VERSION_NTF(void* host, YCDataHolder* holder)
{
	GGPKG_VERSION_NTF* pkg = (GGPKG_VERSION_NTF*)host;
	if (-1 == holder->addDword(pkg->GGVersion)) {
		return -1;
	}
	if (-1 == holder->addDword(pkg->GSVersion)) {
		return -1;
	}
	return holder->getPosition();
}

int Decode_GGPKG_VERSION_RPT(void* host, YCDataHolder* holder)
{
	GGPKG_VERSION_RPT* pkg = (GGPKG_VERSION_RPT*)host;
	if (-1 == holder->delDword(pkg->version)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_GGPKG_VERSION_NTF(void* host, YCDataHolder* holder)
{
	GGPKG_VERSION_NTF* pkg = (GGPKG_VERSION_NTF*)host;
	if (-1 == holder->delDword(pkg->GGVersion)) {
		return -1;
	}
	if (-1 == holder->delDword(pkg->GSVersion)) {
		return -1;
	}
	return holder->getPosition();
}


/////////////////////////////////////////////////////////
// YCPkg_0001_Version 初始化函数实现
/////////////////////////////////////////////////////////
void YCPkg_0001_Version_init(YCNetwork* network)
{
	network->registry(1, Encode_GGPKG_VERSION_RPT, Decode_GGPKG_VERSION_RPT);
	network->registry(2, Encode_GGPKG_VERSION_NTF, Decode_GGPKG_VERSION_NTF);
}

