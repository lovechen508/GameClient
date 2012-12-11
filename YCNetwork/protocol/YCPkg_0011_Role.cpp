#include "YCPkg_0011_Role.h"

#include "..\YCNetwork.h"
#include "..\YCDataHolder.h"

int Encode_AA(void* host, YCDataHolder* holder)
{
	AA* pkg = (AA*)host;
	return holder->getPosition();
}
int Encode_CC(void* host, YCDataHolder* holder)
{
	CC* pkg = (CC*)host;
	return holder->getPosition();
}
int Encode_EE(void* host, YCDataHolder* holder)
{
	EE* pkg = (EE*)host;
	return holder->getPosition();
}
int Encode_GG(void* host, YCDataHolder* holder)
{
	GG* pkg = (GG*)host;
	return holder->getPosition();
}
int Encode_II(void* host, YCDataHolder* holder)
{
	II* pkg = (II*)host;
	return holder->getPosition();
}
int Encode_GGPKG_ROLELIST_NTF(void* host, YCDataHolder* holder)
{
	GGPKG_ROLELIST_NTF* pkg = (GGPKG_ROLELIST_NTF*)host;
	return holder->getPosition();
}
int Encode_BB(void* host, YCDataHolder* holder)
{
	BB* pkg = (BB*)host;
	return holder->getPosition();
}
int Encode_DD(void* host, YCDataHolder* holder)
{
	DD* pkg = (DD*)host;
	return holder->getPosition();
}
int Encode_FF(void* host, YCDataHolder* holder)
{
	FF* pkg = (FF*)host;
	return holder->getPosition();
}
int Encode_HH(void* host, YCDataHolder* holder)
{
	HH* pkg = (HH*)host;
	return holder->getPosition();
}
int Encode_JJ(void* host, YCDataHolder* holder)
{
	JJ* pkg = (JJ*)host;
	return holder->getPosition();
}

int Decode_AA(void* host, YCDataHolder* holder)
{
	AA* pkg = (AA*)host;
	return holder->getPosition();
}
int Decode_CC(void* host, YCDataHolder* holder)
{
	CC* pkg = (CC*)host;
	return holder->getPosition();
}
int Decode_EE(void* host, YCDataHolder* holder)
{
	EE* pkg = (EE*)host;
	return holder->getPosition();
}
int Decode_GG(void* host, YCDataHolder* holder)
{
	GG* pkg = (GG*)host;
	return holder->getPosition();
}
int Decode_II(void* host, YCDataHolder* holder)
{
	II* pkg = (II*)host;
	return holder->getPosition();
}
int Decode_GGPKG_ROLELIST_NTF(void* host, YCDataHolder* holder)
{
	GGPKG_ROLELIST_NTF* pkg = (GGPKG_ROLELIST_NTF*)host;
	return holder->getPosition();
}
int Decode_BB(void* host, YCDataHolder* holder)
{
	BB* pkg = (BB*)host;
	return holder->getPosition();
}
int Decode_DD(void* host, YCDataHolder* holder)
{
	DD* pkg = (DD*)host;
	return holder->getPosition();
}
int Decode_FF(void* host, YCDataHolder* holder)
{
	FF* pkg = (FF*)host;
	return holder->getPosition();
}
int Decode_HH(void* host, YCDataHolder* holder)
{
	HH* pkg = (HH*)host;
	return holder->getPosition();
}
int Decode_JJ(void* host, YCDataHolder* holder)
{
	JJ* pkg = (JJ*)host;
	return holder->getPosition();
}


/////////////////////////////////////////////////////////
// YCPkg_0011_Role 初始化函数实现
/////////////////////////////////////////////////////////
void YCPkg_0011_Role_init(YCNetwork* network)
{
	network->registry(12, Encode_AA, Decode_AA);
	network->registry(14, Encode_CC, Decode_CC);
	network->registry(16, Encode_EE, Decode_EE);
	network->registry(18, Encode_GG, Decode_GG);
	network->registry(20, Encode_II, Decode_II);
	network->registry(11, Encode_GGPKG_ROLELIST_NTF, Decode_GGPKG_ROLELIST_NTF);
	network->registry(13, Encode_BB, Decode_BB);
	network->registry(15, Encode_DD, Decode_DD);
	network->registry(17, Encode_FF, Decode_FF);
	network->registry(19, Encode_HH, Decode_HH);
	network->registry(21, Encode_JJ, Decode_JJ);
}

