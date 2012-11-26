#include "YCIMessageUnit.h"

#include "YCIUnitAction.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCIMessageUnit::YCIMessageUnit(void)
	: myMouseClickAction(NULL)
	, myNext(NULL)
{
}

YCIMessageUnit::~YCIMessageUnit(void)
{
	SAFE_DELETE(myMouseClickAction);
	SAFE_DELETE(myNext);
}

//
// ������bindMouseClick(YCIUnitAction* action)
//
// Ŀ�ģ���������¼�
//
void YCIMessageUnit::bindMouseClick(YCIUnitAction* action)
{
	SMART_ASSERT(action != NULL);
	myMouseClickAction = action;
}

//
// ������isMouseClickEnabled()
//
// Ŀ�ģ�����Ѱ�������¼�������true
//      ���򷵻�false
//
bool YCIMessageUnit::isMouseClickEnabled()
{
	return (myMouseClickAction != NULL);
}

//
// ������onMouseClick()
//
// Ŀ�ģ�������¼�����
//
void YCIMessageUnit::onMouseClick()
{
	if (myMouseClickAction != NULL)
	{
		myMouseClickAction->onMouseClick();
	}
}

//
// ������append(YCIMessageUnit* next)
//
// Ŀ�ģ�׷����Ϣ��Ԫ
//
void YCIMessageUnit::append(YCIMessageUnit* next)
{
	SMART_ASSERT(next != NULL);
	if (NULL == next)
	{
		throw YCException(2002, "YCIMessageUnit::append����nextΪ��");
	}

	myNext = next;
}

//
// ������next() const
//
// Ŀ�ģ������Ϣ��Ԫ
//
YCIMessageUnit* YCIMessageUnit::next()
{
	return myNext;
}

const YCIMessageUnit* YCIMessageUnit::next() const
{
	return myNext;
}
