#ifndef _INCLUDE_YCCHATMSGDEFINE_H_
#define _INCLUDE_YCCHATMSGDEFINE_H_

#include "YCObjectDef.h"

//
// ����Ƶ��
//
enum {
	CH_LIGHT	= 1,	//����
	CH_PRIVATE	= 2,	//˽��
	CH_FRIEND	= 4,	//����
	CH_TEAM		= 8,	//���
	CH_FAMILY	= 16,	//����
	CH_COUNTRY	= 32,	//����
	CH_WORLD	= 64,	//����
	CH_SYSTEM	= 128,	//ϵͳ
	CH_GLOBAL	= 256,	//����
};

#define CH_FILTER_ID 1
#define BLACK_FILTER_ID   2

#endif