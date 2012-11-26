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
// ������setCapacity/getCapacity
//
// Ŀ�ģ����ã���ȡ����
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
// ������findById(unsigned long long id)
//
// Ŀ�ģ�����ID������Ʒ
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
// ������setItemAt(unsigned int index, YCItem* item)
//
// Ŀ�ģ������Ʒ��Package��
//
void YCIPackage::setItemAt(unsigned int index, YCItem* item)
{
	if (index >= myCapacity)
	{
		throw YCException(2002, "YCIPackage::setItemAt��Ʒ�������");
	}

	if (NULL == item)
	{
		throw YCException(2002, "YCIPackage::setItemAt��ƷΪ�գ�");
	}

	Item_List_T* pos = myItems->begin();
	for (; pos != myItems->end(); pos = myItems->next(pos))
	{
		YCItem* theItem = (YCItem*)myItems->payload(pos);
		if (theItem->getIndex() == index)
		{
			throw YCException(2002, "YCIPackage::setItemAt����Index�ظ�");
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
// ������getItemAt(unsigned int index) const
//
// Ŀ�ģ���ȡĳλ�õ���Ʒ
//
YCItem* YCIPackage::getItemAt(unsigned int index)
{
	YCItem* result = NULL;

	if (index >= myCapacity)
	{
		throw YCException(2002, "YCIPackage::getItemAt��Ʒ�������");
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
// ������removeItemAt(unsigned int index)
//
// Ŀ�ģ���ĳλ���Ƴ���Ʒ
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
// ������clear()
//
// Ŀ�ģ����
//
void YCIPackage::clear()
{
	myItems->clear(YCDListClearHelper<YCItem>);
}
