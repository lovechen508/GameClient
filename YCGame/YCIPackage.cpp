#include "YCIPackage.h"

#include "YCItem.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCException.h"

YCIPackage::YCIPackage(void)
	: myItems(new YCDList())
{
}


YCIPackage::~YCIPackage(void)
{
	SAFE_DELETE(myItems);
}

//
// 函数：setCapacity/getCapacity
//
// 目的：设置，获取容量
// 
void YCIPackage::setCapacity(unsigned int n)
{
	myCapacity = n;
}

unsigned int YCIPackage::getCapacity() const
{
	return myCapacity;
}

//
// 函数：findById(unsigned long long id)
//
// 目的：根据ID查找物品
//
YCItem * YCIPackage::findById(unsigned long long id)
{
	YCItem * result = NULL;

	Item_List_T* pos = myItems->begin();
	for (; pos != myItems->end(); pos = myItems->next(pos))
	{
		YCItem* item = (YCItem*)myItems->payload(pos);
		if (item->getId() == id)
		{
			break;
		}
	}

	if (pos != NULL)
	{
		result = (YCItem*)myItems->remove(pos);
	}

	return result;
}

//
// 函数：setItemAt(unsigned int index, YCItem* item)
//
// 目的：添加物品到Package中
//
void YCIPackage::setItemAt(unsigned int index, YCItem* item)
{
	if (index >= myCapacity)
	{
		throw YCException(2002, "YCIPackage::setItemAt物品编号有误！");
	}

	if (NULL == item)
	{
		throw YCException(2002, "YCIPackage::setItemAt物品为空！");
	}

	Item_List_T* pos = myItems->begin();
	for (; pos != myItems->end(); pos = myItems->next(pos))
	{
		YCItem* theItem = (YCItem*)myItems->payload(pos);
		if (theItem->getIndex() == index)
		{
			throw YCException(2002, "YCIPackage::setItemAt设置Index重复");
		}
		else if (theItem->getIndex() > index)
		{
			break;
		}
	}

	item->addToPackage(this);
	item->setIndex(index);
	myItems->insert(item, pos);
}

//
// 函数：getItemAt(unsigned int index) const
//
// 目的：获取某位置的物品
//
YCItem* YCIPackage::getItemAt(unsigned int index)
{
	YCItem* result = NULL;

	if (index >= myCapacity)
	{
		throw YCException(2002, "YCIPackage::getItemAt物品编号有误！");
	}

	for (Item_List_T* item = myItems->begin();
		 item != myItems->end();
		 item = myItems->next(item))
	{
		YCItem* theItem = (YCItem*)myItems->payload(item);
		if (theItem->getIndex() == index)
		{
			result = theItem;
			break;
		}
		else if (theItem->getIndex() > index)
		{
			break;
		}
	}

	return result;
}

//
// 函数：removeItemAt(unsigned int index)
//
// 目的：从某位置移除物品
//
YCItem* YCIPackage::removeItemAt(unsigned int index)
{
	YCItem* result = NULL;

	Item_List_T* pos = myItems->begin();
	for (; pos != myItems->end(); pos = myItems->next(pos))
	{
		YCItem* item = (YCItem*)myItems->payload((pos));
		if (item->getIndex() == index)
		{
			result = item;
			break;
		}
	}

	if (pos != NULL)
	{
		myItems->remove(pos);
	}

	return result;
}

//
// 函数：clear()
//
// 目的：清空
//
void YCIPackage::clear()
{
	myItems->clear(YCDListClearHelper<YCItem>);
}
