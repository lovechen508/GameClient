#ifndef _INCLUDE_YCPkg_1004_Appear_H_
#define _INCLUDE_YCPkg_1004_Appear_H_

#include "..\YCType2Int.h"

class YCNetwork;
class YCDataHolder;

#define GS_MAPNAME_LEN	64
#define ROLE_NAME_LEN	64

typedef struct ROLE_BASIC_INFO {
	char		roleName[ROLE_NAME_LEN];
	unsigned int	objectId;
	unsigned short	level;
	unsigned char	direction;
	unsigned short	x;
	unsigned short	y;
	unsigned char	career;
	unsigned char	sex;
} ROLE_BASIC_INFO;
typedef struct ROLE_ADDITION_INFO {
	unsigned char	portrait;
	unsigned char	hairType;
	unsigned short	helmId;
} ROLE_ADDITION_INFO;



typedef struct GSPKG_ROLEMAINDATA_NTF {
	ROLE_BASIC_INFO	basic;
	ROLE_ADDITION_INFO	addition;
} GSPKG_ROLEMAINDATA_NTF;


typedef struct GSPKG_ROLEPOSITION_NTF {
	unsigned short	wMapCode;
	char		szMapName[GS_MAPNAME_LEN];
	unsigned short	x;
	unsigned short	y;
	unsigned char	byDirection;
} GSPKG_ROLEPOSITION_NTF;


typedef struct GSPKG_ROLEAPPEAR_NTF {
	ROLE_BASIC_INFO	basic;
	ROLE_ADDITION_INFO	addition;
} GSPKG_ROLEAPPEAR_NTF;


typedef struct GSPKG_MONSTERAPPEAR_NTF {
} GSPKG_MONSTERAPPEAR_NTF;


typedef struct GSPKG_NPCAPPEAR_NTF {
} GSPKG_NPCAPPEAR_NTF;


typedef struct GSPKG_ITEMAPPEAR_NTF {
} GSPKG_ITEMAPPEAR_NTF;


typedef struct GSPKG_MAGICAPPEAR_NTF {
} GSPKG_MAGICAPPEAR_NTF;


typedef struct GSPKG_OBJECTDISAPPEAR_NTF {
} GSPKG_OBJECTDISAPPEAR_NTF;


template<> struct YCType2Int<GSPKG_ROLEMAINDATA_NTF>{enum{id=1004};};
template<> struct YCType2Int<GSPKG_ROLEPOSITION_NTF>{enum{id=1005};};
template<> struct YCType2Int<GSPKG_ROLEAPPEAR_NTF>{enum{id=1006};};
template<> struct YCType2Int<GSPKG_MONSTERAPPEAR_NTF>{enum{id=1007};};
template<> struct YCType2Int<GSPKG_NPCAPPEAR_NTF>{enum{id=1008};};
template<> struct YCType2Int<GSPKG_ITEMAPPEAR_NTF>{enum{id=1009};};
template<> struct YCType2Int<GSPKG_MAGICAPPEAR_NTF>{enum{id=1010};};
template<> struct YCType2Int<GSPKG_OBJECTDISAPPEAR_NTF>{enum{id=1011};};



/////////////////////////////////////////////////////////
// YCPkg_1004_Appear 初始化函数声明
/////////////////////////////////////////////////////////
void YCPkg_1004_Appear_init(YCNetwork* network);

#endif
