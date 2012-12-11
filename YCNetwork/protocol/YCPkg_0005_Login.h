#ifndef _INCLUDE_YCPkg_0005_Login_H_
#define _INCLUDE_YCPkg_0005_Login_H_

#include "..\YCType2Int.h"

class YCNetwork;
class YCDataHolder;


typedef struct GGPKG_LOGIN_REQ {
} GGPKG_LOGIN_REQ;



typedef struct GGPKG_LOGIN_ACK {
} GGPKG_LOGIN_ACK;


template<> struct YCType2Int<GGPKG_LOGIN_REQ>{enum{id=5};};
template<> struct YCType2Int<GGPKG_LOGIN_ACK>{enum{id=6};};

static int Encode_GGPKG_LOGIN_REQ(void* host, YCDataHolder* holder);
static int Decode_GGPKG_LOGIN_REQ(void* host, YCDataHolder* holder);
static int Encode_GGPKG_LOGIN_ACK(void* host, YCDataHolder* holder);
static int Decode_GGPKG_LOGIN_ACK(void* host, YCDataHolder* holder);


/////////////////////////////////////////////////////////
// YCPkg_0005_Login 初始化函数声明
/////////////////////////////////////////////////////////
static void YCPkg_0005_Login_init(YCNetwork* network);

#endif
