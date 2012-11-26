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
// 函数：bindMouseClick(YCIUnitAction* action)
//
// 目的：绑定鼠标点击事件
//
void YCIMessageUnit::bindMouseClick(YCIUnitAction* action)
{
	SMART_ASSERT(action != NULL);
	myMouseClickAction = action;
}

//
// 函数：isMouseClickEnabled()
//
// 目的：如果已绑定鼠标点击事件，返回true
//      否则返回false
//
bool YCIMessageUnit::isMouseClickEnabled()
{
	return (myMouseClickAction != NULL);
}

//
// 函数：onMouseClick()
//
// 目的：鼠标点击事件到达
//
void YCIMessageUnit::onMouseClick()
{
	if (myMouseClickAction != NULL)
	{
		myMouseClickAction->onMouseClick();
	}
}

//
// 函数：append(YCIMessageUnit* next)
//
// 目的：追加消息单元
//
void YCIMessageUnit::append(YCIMessageUnit* next)
{
	SMART_ASSERT(next != NULL);
	if (NULL == next)
	{
		throw YCException(2002, "YCIMessageUnit::append设置next为空");
	}

	myNext = next;
}

//
// 函数：next() const
//
// 目的：后继消息单元
//
YCIMessageUnit* YCIMessageUnit::next()
{
	return myNext;
}

const YCIMessageUnit* YCIMessageUnit::next() const
{
	return myNext;
}
