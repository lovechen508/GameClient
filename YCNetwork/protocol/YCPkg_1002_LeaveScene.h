#ifndef _INCLUDE_YCPkg_1002_LeaveScene_H_
#define _INCLUDE_YCPkg_1002_LeaveScene_H_

#include "..\YCType2Int.h"

class YCNetwork;
class YCDataHolder;


typedef struct GSMSG_LEAVEGAME_REQ {
	unsigned char	byType;
} GSMSG_LEAVEGAME_REQ;



typedef struct GSMSG_LEAVEGAME_NTF {
	int	nErrCode;
	unsigned char	byType;
} GSMSG_LEAVEGAME_NTF;


template<> struct YCType2Int<GSMSG_LEAVEGAME_REQ>{enum{id=1002};};
template<> struct YCType2Int<GSMSG_LEAVEGAME_NTF>{enum{id=1003};};

static int Encode_GSMSG_LEAVEGAME_REQ(void* host, YCDataHolder* holder);
static int Decode_GSMSG_LEAVEGAME_REQ(void* host, YCDataHolder* holder);
static int Encode_GSMSG_LEAVEGAME_NTF(void* host, YCDataHolder* holder);
static int Decode_GSMSG_LEAVEGAME_NTF(void* host, YCDataHolder* holder);


/////////////////////////////////////////////////////////
// YCPkg_1002_LeaveScene 初始化函数声明
/////////////////////////////////////////////////////////
static void YCPkg_1002_LeaveScene_init(YCNetwork* network);

#endif
