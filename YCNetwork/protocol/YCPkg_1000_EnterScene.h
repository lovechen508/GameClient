#ifndef _INCLUDE_YCPkg_1000_EnterScene_H_
#define _INCLUDE_YCPkg_1000_EnterScene_H_

#include "..\YCType2Int.h"

class YCNetwork;
class YCDataHolder;

#include "YCPkg_1004_Appear.h"


typedef struct GSMSG_ENTERGAME_REQ {
	char		szRolename[ROLE_NAME_LEN];
	unsigned long long	sessionId;
	unsigned int	dwFlags;
} GSMSG_ENTERGAME_REQ;



typedef struct GSMSG_ENTERGAME_NTF {
	int	nErrCode;
	unsigned int	dwFlags;
} GSMSG_ENTERGAME_NTF;


template<> struct YCType2Int<GSMSG_ENTERGAME_REQ>{enum{id=1000};};
template<> struct YCType2Int<GSMSG_ENTERGAME_NTF>{enum{id=1001};};



/////////////////////////////////////////////////////////
// YCPkg_1000_EnterScene 初始化函数声明
/////////////////////////////////////////////////////////
void YCPkg_1000_EnterScene_init(YCNetwork* network);

#endif
