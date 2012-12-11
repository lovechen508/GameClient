#include "YCPkg_0022_Ping.h"

#include "..\YCNetwork.h"
#include "..\YCDataHolder.h"

int Encode_GGPKG_PINT_REQ(void* host, YCDataHolder* holder)
{
	GGPKG_PINT_REQ* pkg = (GGPKG_PINT_REQ*)host;
	return holder->getPosition();
}
int Encode_GGPKG_PING_ACK(void* host, YCDataHolder* holder)
{
	GGPKG_PING_ACK* pkg = (GGPKG_PING_ACK*)host;
	return holder->getPosition();
}

int Decode_GGPKG_PINT_REQ(void* host, YCDataHolder* holder)
{
	GGPKG_PINT_REQ* pkg = (GGPKG_PINT_REQ*)host;
	return holder->getPosition();
}
int Decode_GGPKG_PING_ACK(void* host, YCDataHolder* holder)
{
	GGPKG_PING_ACK* pkg = (GGPKG_PING_ACK*)host;
	return holder->getPosition();
}


/////////////////////////////////////////////////////////
// YCPkg_0022_Ping 初始化函数实现
/////////////////////////////////////////////////////////
void YCPkg_0022_Ping_init(YCNetwork* network)
{
	network->registry(22, Encode_GGPKG_PINT_REQ, Decode_GGPKG_PINT_REQ);
	network->registry(23, Encode_GGPKG_PING_ACK, Decode_GGPKG_PING_ACK);
}

