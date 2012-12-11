#ifndef _INCLUDE_YCPkg_0090_Npc_H_
#define _INCLUDE_YCPkg_0090_Npc_H_

#include "..\YCType2Int.h"

class YCNetwork;
class YCDataHolder;

#define MAX_NPC_MSG_LEN	10240


typedef struct GSMSG_TALKNPC_REQ {
	unsigned int	dwObjectId;
} GSMSG_TALKNPC_REQ;


typedef struct GSMSG_NPCSELECTION_REQ {
	unsigned short	wSelection;
} GSMSG_NPCSELECTION_REQ;



typedef struct GSMSG_TALKNPC_NTF {
	unsigned int	dwObjectId;
	unsigned short	wLen;
	char		szMessage[MAX_NPC_MSG_LEN];
} GSMSG_TALKNPC_NTF;


template<> struct YCType2Int<GSMSG_TALKNPC_REQ>{enum{id=81};};
template<> struct YCType2Int<GSMSG_NPCSELECTION_REQ>{enum{id=83};};
template<> struct YCType2Int<GSMSG_TALKNPC_NTF>{enum{id=82};};

static int Encode_GSMSG_TALKNPC_REQ(void* host, YCDataHolder* holder);
static int Decode_GSMSG_TALKNPC_REQ(void* host, YCDataHolder* holder);
static int Encode_GSMSG_NPCSELECTION_REQ(void* host, YCDataHolder* holder);
static int Decode_GSMSG_NPCSELECTION_REQ(void* host, YCDataHolder* holder);
static int Encode_GSMSG_TALKNPC_NTF(void* host, YCDataHolder* holder);
static int Decode_GSMSG_TALKNPC_NTF(void* host, YCDataHolder* holder);


/////////////////////////////////////////////////////////
// YCPkg_0090_Npc 初始化函数声明
/////////////////////////////////////////////////////////
static void YCPkg_0090_Npc_init(YCNetwork* network);

#endif
