#ifndef _INCLUDE_YCCHATMSGDEFINE_H_
#define _INCLUDE_YCCHATMSGDEFINE_H_

#include "YCObjectDef.h"

//
// 聊天频道
//
enum {
	CH_LIGHT	= 1,	//轻声
	CH_PRIVATE	= 2,	//私聊
	CH_FRIEND	= 4,	//好友
	CH_TEAM		= 8,	//组队
	CH_FAMILY	= 16,	//家族
	CH_COUNTRY	= 32,	//国家
	CH_WORLD	= 64,	//世界
	CH_SYSTEM	= 128,	//系统
	CH_GLOBAL	= 256,	//彩世
};

#define CH_FILTER_ID 1
#define BLACK_FILTER_ID   2

#endif