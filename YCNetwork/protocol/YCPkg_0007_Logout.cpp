#include "YCPkg_0007_Logout.h"

#include "..\YCNetwork.h"
#include "..\YCDataHolder.h"

int Encode_GGPKG_LOGOUT_REQ(void* host, YCDataHolder* holder)
{
	GGPKG_LOGOUT_REQ* pkg = (GGPKG_LOGOUT_REQ*)host;
	return holder->getPosition();
}
int Encode_GGPKG_LOGOUT_ACK(void* host, YCDataHolder* holder)
{
	GGPKG_LOGOUT_ACK* pkg = (GGPKG_LOGOUT_ACK*)host;
	return holder->getPosition();
}

int Decode_GGPKG_LOGOUT_REQ(void* host, YCDataHolder* holder)
{
	GGPKG_LOGOUT_REQ* pkg = (GGPKG_LOGOUT_REQ*)host;
	return holder->getPosition();
}
int Decode_GGPKG_LOGOUT_ACK(void* host, YCDataHolder* holder)
{
	GGPKG_LOGOUT_ACK* pkg = (GGPKG_LOGOUT_ACK*)host;
	return holder->getPosition();
}


/////////////////////////////////////////////////////////
// YCPkg_0007_Logout 初始化函数实现
/////////////////////////////////////////////////////////
void YCPkg_0007_Logout_init(YCNetwork* network)
{
	network->registry(7, Encode_GGPKG_LOGOUT_REQ, Decode_GGPKG_LOGOUT_REQ);
	network->registry(8, Encode_GGPKG_LOGOUT_ACK, Decode_GGPKG_LOGOUT_ACK);
}

