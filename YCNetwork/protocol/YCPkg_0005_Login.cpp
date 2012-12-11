#include "YCPkg_0005_Login.h"

#include "..\YCNetwork.h"
#include "..\YCDataHolder.h"

int Encode_GGPKG_LOGIN_REQ(void* host, YCDataHolder* holder)
{
	GGPKG_LOGIN_REQ* pkg = (GGPKG_LOGIN_REQ*)host;
	return holder->getPosition();
}
int Encode_GGPKG_LOGIN_ACK(void* host, YCDataHolder* holder)
{
	GGPKG_LOGIN_ACK* pkg = (GGPKG_LOGIN_ACK*)host;
	return holder->getPosition();
}

int Decode_GGPKG_LOGIN_REQ(void* host, YCDataHolder* holder)
{
	GGPKG_LOGIN_REQ* pkg = (GGPKG_LOGIN_REQ*)host;
	return holder->getPosition();
}
int Decode_GGPKG_LOGIN_ACK(void* host, YCDataHolder* holder)
{
	GGPKG_LOGIN_ACK* pkg = (GGPKG_LOGIN_ACK*)host;
	return holder->getPosition();
}


/////////////////////////////////////////////////////////
// YCPkg_0005_Login 初始化函数实现
/////////////////////////////////////////////////////////
void YCPkg_0005_Login_init(YCNetwork* network)
{
	network->registry(5, Encode_GGPKG_LOGIN_REQ, Decode_GGPKG_LOGIN_REQ);
	network->registry(6, Encode_GGPKG_LOGIN_ACK, Decode_GGPKG_LOGIN_ACK);
}

