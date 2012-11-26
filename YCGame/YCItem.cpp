#include "YCItem.h"

#include "YCIPackage.h"

#include "YCBasic\YCException.h"

YCItem::YCItem(void)
	: myPackage(NULL)
	, myIndex(0)
{
}

YCItem::~YCItem(void)
{
}

//
// 函数：add2Package(YCIPackage* package)
//
// 目的：设置物品所在包裹
//
void YCItem::addToPackage(YCIPackage* package)
{
	if (package == NULL)
	{
		throw YCException(2002, "YCItem::addToPackage包裹为空");
	}

	myPackage = package;
}

void YCItem::removeFromPackage()
{
	myPackage = NULL;
}

//
// 函数：setIndex(unsigned int index)
//       getIndex() const
//
// 目的：设置，获取物品在包裹中的位置
//
void YCItem::setIndex(unsigned int index)
{
	if (myPackage == NULL)
	{
		throw YCException(2002, "YCItem::setIndex物品不在包裹中！");
	}

	if (index >= myPackage->getCapacity())
	{
		throw YCException(2002, "YCItem::setIndex物品编号过大");
	}

	myIndex = index;
}

unsigned int YCItem::getIndex() const
{
	if (myPackage == NULL)
	{
		throw YCException(2002, "YCItem::getIndex物品不在包裹中！");
	}

	return myIndex;
}
