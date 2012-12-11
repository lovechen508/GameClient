#ifndef _INCLUDE_YCPkg_0003_Error_H_
#define _INCLUDE_YCPkg_0003_Error_H_

#include "..\YCType2Int.h"

class YCNetwork;
class YCDataHolder;


typedef struct GGPKG_ERROR_RPT {
	int	nErrCode;
} GGPKG_ERROR_RPT;



typedef struct GGPKG_ERROR_NTF {
	int	nErrCode;
	char		szErrMsg[255];
} GGPKG_ERROR_NTF;


template<> struct YCType2Int<GGPKG_ERROR_RPT>{enum{id=3};};
template<> struct YCType2Int<GGPKG_ERROR_NTF>{enum{id=4};};



/////////////////////////////////////////////////////////
// YCPkg_0003_Error 初始化函数声明
/////////////////////////////////////////////////////////
void YCPkg_0003_Error_init(YCNetwork* network);

#endif
