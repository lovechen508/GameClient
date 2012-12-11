#ifndef _INCLUDE_YCPkg_0011_Role_H_
#define _INCLUDE_YCPkg_0011_Role_H_

#include "..\YCType2Int.h"

class YCNetwork;
class YCDataHolder;


typedef struct AA {
} AA;


typedef struct CC {
} CC;


typedef struct EE {
} EE;


typedef struct GG {
} GG;


typedef struct II {
} II;



typedef struct GGPKG_ROLELIST_NTF {
} GGPKG_ROLELIST_NTF;


typedef struct BB {
} BB;


typedef struct DD {
} DD;


typedef struct FF {
} FF;


typedef struct HH {
} HH;


typedef struct JJ {
} JJ;


template<> struct YCType2Int<AA>{enum{id=12};};
template<> struct YCType2Int<CC>{enum{id=14};};
template<> struct YCType2Int<EE>{enum{id=16};};
template<> struct YCType2Int<GG>{enum{id=18};};
template<> struct YCType2Int<II>{enum{id=20};};
template<> struct YCType2Int<GGPKG_ROLELIST_NTF>{enum{id=11};};
template<> struct YCType2Int<BB>{enum{id=13};};
template<> struct YCType2Int<DD>{enum{id=15};};
template<> struct YCType2Int<FF>{enum{id=17};};
template<> struct YCType2Int<HH>{enum{id=19};};
template<> struct YCType2Int<JJ>{enum{id=21};};



/////////////////////////////////////////////////////////
// YCPkg_0011_Role 初始化函数声明
/////////////////////////////////////////////////////////
void YCPkg_0011_Role_init(YCNetwork* network);

#endif
