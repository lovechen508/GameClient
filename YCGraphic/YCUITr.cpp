#include "YCUITr.h"

#include <list>

#include "YCUITd.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"

YCUITr::YCUITr(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
{
}


YCUITr::~YCUITr(void)
{
}
	
//
// ������layout(bool resizable)
//
// Ŀ�ģ�����Ԫ��������Ԫ���Ĳ���
//
void YCUITr::layout(bool resizable)
{
	// ��� background
	buildBackground();
		
	int leftTopX = myMarginLeft + myPaddingLeft;
	int leftTopY = myMarginTop + myPaddingTop;

	//
	// �ڲ��ֹ����в��ַ�ʽ�����ҽ���
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
			leftTopX += tag->getWidthWithMargin();
			tag->layout(resizable);

			YCUITd* td = (YCUITd*)tag;
			if (mySubTags->next(item) != NULL)
			{
				td->setDelimiterEnable(true);
			}
			else
			{
				td->setDelimiterEnable(false);
			}
		}
	}
}


//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// ������validateSubTag(const char* name)
//
// Ŀ�ģ�У���ӿؼ�
//
bool YCUITr::validateSubTag(const char* name)
{
	SMART_ASSERT(name != NULL);
	return (strcmp("td", name) == 0);
}