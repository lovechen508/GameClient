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
			throw YCException(2002, "YCChatMessage��Ч��Ϣ��");
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
// ������setUnit(YCIMessageUnit* unit)
//
// Ŀ�ģ�������Ϣ��Ԫ���ӹ���������
//
void YCChatMessage::setUnit(YCIMessageUnit* unit)
{
	if (NULL == unit)
	{
		throw YCException(2002, "YCChatMessage::setUnit������Ϣ��ԪΪ��");
	}

	myUnits = unit;
}

//
// ������getUnit()
//
// Ŀ�ģ� ȡ����Ϣ��Ԫ
//
YCIMessageUnit* YCChatMessage::getUnit()
{
	return myUnits;
}
