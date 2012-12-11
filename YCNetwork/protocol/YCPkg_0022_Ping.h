#ifndef _INCLUDE_YCPkg_0022_Ping_H_
#define _INCLUDE_YCPkg_0022_Ping_H_

#include "..\YCType2Int.h"

class YCNetwork;
class YCDataHolder;


typedef struct GGPKG_PINT_REQ {
} GGPKG_PINT_REQ;



typedef struct GGPKG_PING_ACK {
} GGPKG_PING_ACK;


template<> struct YCType2Int<GGPKG_PINT_REQ>{enum{id=22};};
template<> struct YCType2Int<GGPKG_PING_ACK>{enum{id=23};};



/////////////////////////////////////////////////////////
// YCPkg_0022_Ping 初始化函数声明
/////////////////////////////////////////////////////////
void YCPkg_0022_Ping_init(YCNetwork* network);

#endif
