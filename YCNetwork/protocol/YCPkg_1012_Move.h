#ifndef _INCLUDE_YCPkg_1012_Move_H_
#define _INCLUDE_YCPkg_1012_Move_H_

#include "..\YCType2Int.h"

class YCNetwork;
class YCDataHolder;


typedef struct GSPKG_MOVE_REQ {
	unsigned char	byType;
	unsigned char	byDirection;
} GSPKG_MOVE_REQ;



typedef struct GSPKG_MOVE_ACK {
	int	nErrCode;
	unsigned char	byType;
	unsigned short	x;
	unsigned short	y;
} GSPKG_MOVE_ACK;


template<> struct YCType2Int<GSPKG_MOVE_REQ>{enum{id=1012};};
template<> struct YCType2Int<GSPKG_MOVE_ACK>{enum{id=1013};};

static int Encode_GSPKG_MOVE_REQ(void* host, YCDataHolder* holder);
static int Decode_GSPKG_MOVE_REQ(void* host, YCDataHolder* holder);
static int Encode_GSPKG_MOVE_ACK(void* host, YCDataHolder* holder);
static int Decode_GSPKG_MOVE_ACK(void* host, YCDataHolder* holder);


/////////////////////////////////////////////////////////
// YCPkg_1012_Move 初始化函数声明
/////////////////////////////////////////////////////////
static void YCPkg_1012_Move_init(YCNetwork* network);

#endif
