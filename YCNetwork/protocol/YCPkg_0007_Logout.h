#ifndef _INCLUDE_YCPkg_0007_Logout_H_
#define _INCLUDE_YCPkg_0007_Logout_H_

#include "..\YCType2Int.h"

class YCNetwork;
class YCDataHolder;


typedef struct GGPKG_LOGOUT_REQ {
} GGPKG_LOGOUT_REQ;



typedef struct GGPKG_LOGOUT_ACK {
} GGPKG_LOGOUT_ACK;


template<> struct YCType2Int<GGPKG_LOGOUT_REQ>{enum{id=7};};
template<> struct YCType2Int<GGPKG_LOGOUT_ACK>{enum{id=8};};



/////////////////////////////////////////////////////////
// YCPkg_0007_Logout 初始化函数声明
/////////////////////////////////////////////////////////
void YCPkg_0007_Logout_init(YCNetwork* network);

#endif
