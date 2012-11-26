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
// ������add2Package(YCIPackage* package)
//
// Ŀ�ģ�������Ʒ���ڰ���
//
void YCItem::addToPackage(YCIPackage* package)
{
	if (package == NULL)
	{
		throw YCException(2002, "YCItem::addToPackage����Ϊ��");
	}

	myPackage = package;
}

void YCItem::removeFromPackage()
{
	myPackage = NULL;
}

//
// ������setIndex(unsigned int index)
//       getIndex() const
//
// Ŀ�ģ����ã���ȡ��Ʒ�ڰ����е�λ��
//
void YCItem::setIndex(unsigned int index)
{
	if (myPackage == NULL)
	{
		throw YCException(2002, "YCItem::setIndex��Ʒ���ڰ����У�");
	}

	if (index >= myPackage->getCapacity())
	{
		throw YCException(2002, "YCItem::setIndex��Ʒ��Ź���");
	}

	myIndex = index;
}

unsigned int YCItem::getIndex() const
{
	if (myPackage == NULL)
	{
		throw YCException(2002, "YCItem::getIndex��Ʒ���ڰ����У�");
	}

	return myIndex;
}
