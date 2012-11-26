#include "YCChatMessage.h"

#include "YCChatMessage.h"
#include "YCIMessageUnit.h"
#include "YCChatMsgDefine.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCChatMessage::YCChatMessage(int type, const char* name)
	: myType(type)
	, mySender(NULL)
{
	if (name == NULL)
	{
		if (myType != CH_SYSTEM)
		{
			throw YCException(2002, "YCChatMessage无效消息体");
		}
	}
	else
	{
		int len = strlen(name);
		mySender = new char[len+1];
		memcpy(mySender, name, len);
		mySender[len] = '\0';
	}
}


YCChatMessage::~YCChatMessage(void)
{
	SAFE_DELETE(myUnits);
	SAFE_DELETE_ARRAY(mySender);
}

//
// 函数：setUnit(YCIMessageUnit* unit)
//
// 目的：设置消息单元，接管生命周期
//
void YCChatMessage::setUnit(YCIMessageUnit* unit)
{
	if (NULL == unit)
	{
		throw YCException(2002, "YCChatMessage::setUnit设置消息单元为空");
	}

	myUnits = unit;
}

//
// 函数：getUnit()
//
// 目的： 取得消息单元
//
YCIMessageUnit* YCChatMessage::getUnit()
{
	return myUnits;
}
