#include "YCPkg_0003_Error.h"

#include "..\YCNetwork.h"
#include "..\YCDataHolder.h"

int Encode_GGPKG_ERROR_RPT(void* host, YCDataHolder* holder)
{
	GGPKG_ERROR_RPT* pkg = (GGPKG_ERROR_RPT*)host;
	if (-1 == holder->addInt(pkg->nErrCode)) {
		return -1;
	}
	return holder->getPosition();
}
int Encode_GGPKG_ERROR_NTF(void* host, YCDataHolder* holder)
{
	GGPKG_ERROR_NTF* pkg = (GGPKG_ERROR_NTF*)host;
	if (-1 == holder->addInt(pkg->nErrCode)) {
		return -1;
	}
	if (-1 == holder->addString(pkg->szErrMsg, 255)) {
		return -1;
	}
	return holder->getPosition();
}

int Decode_GGPKG_ERROR_RPT(void* host, YCDataHolder* holder)
{
	GGPKG_ERROR_RPT* pkg = (GGPKG_ERROR_RPT*)host;
	if (-1 == holder->delInt(pkg->nErrCode)) {
		return -1;
	}
	return holder->getPosition();
}
int Decode_GGPKG_ERROR_NTF(void* host, YCDataHolder* holder)
{
	GGPKG_ERROR_NTF* pkg = (GGPKG_ERROR_NTF*)host;
	if (-1 == holder->delInt(pkg->nErrCode)) {
		return -1;
	}
	if (-1 == holder->delString(pkg->szErrMsg, 255)) {
		return -1;
	}
	return holder->getPosition();
}


/////////////////////////////////////////////////////////
// YCPkg_0003_Error 初始化函数实现
/////////////////////////////////////////////////////////
void YCPkg_0003_Error_init(YCNetwork* network)
{
	network->registry(3, Encode_GGPKG_ERROR_RPT, Decode_GGPKG_ERROR_RPT);
	network->registry(4, Encode_GGPKG_ERROR_NTF, Decode_GGPKG_ERROR_NTF);
}

