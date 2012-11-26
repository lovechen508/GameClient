#include "YCUITable.h"

#include <list>

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"

YCUITable::YCUITable(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
{
}


YCUITable::~YCUITable(void)
{
}

//
// ������layout(bool resizable)
//
// Ŀ�ģ�����Ԫ��������Ԫ���Ĳ���
//
void YCUITable::layout(bool resizable)
{
	// ��� background
	buildBackground();
		
	int leftTopX = myMarginLeft + myPaddingLeft;
	int leftTopY = myMarginTop + myPaddingTop;

	//
	// �ڲ��ֹ�����
	// ����һ��һ�еĲ��ַ�ʽ���ϵ��½���
	//
	std::list<YCIUITag*> line;
	for (Item_List_T* item = mySubTags->begin();
		 item != mySubTags->end();
		 item = mySubTags->next(item))
	{
		YCIUITag* tag = (YCIUITag*)mySubTags->payload(item);
		if (tag->isVisible())
		{
			tag->setPosition(leftTopX, leftTopY);
			leftTopY += tag->getHeightWithMargin();

			tag->layout(resizable);
		}
	}
}

//
// ������extraDraw()
//
// Ŀ�ģ�UIԪ�����Զ������
//
void YCUITable::extraDraw()
{

}

//
// ������extraCSS()
//
// Ŀ�ģ������ǩ�ض���CSS����
//
void YCUITable::extraCSS()
{

}

//
// ������validateSubTag(const char* name)
//
// Ŀ�ģ�У���ӿؼ�
//
bool YCUITable::validateSubTag(const char* name)
{
	SMART_ASSERT(name != NULL);
	return ((strcmp("tr", name) == 0) || (strcmp("th", name) == 0));
}
