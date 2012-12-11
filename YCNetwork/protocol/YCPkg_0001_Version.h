#ifndef _INCLUDE_YCPkg_0001_Version_H_
#define _INCLUDE_YCPkg_0001_Version_H_

#include "..\YCType2Int.h"

class YCNetwork;
class YCDataHolder;


typedef struct GGPKG_VERSION_RPT {
	unsigned int	version;
} GGPKG_VERSION_RPT;



typedef struct GGPKG_VERSION_NTF {
	unsigned int	GGVersion;
	unsigned int	GSVersion;
} GGPKG_VERSION_NTF;


template<> struct YCType2Int<GGPKG_VERSION_RPT>{enum{id=1};};
template<> struct YCType2Int<GGPKG_VERSION_NTF>{enum{id=2};};



/////////////////////////////////////////////////////////
// YCPkg_0001_Version 初始化函数声明
/////////////////////////////////////////////////////////
void YCPkg_0001_Version_init(YCNetwork* network);

#endif
