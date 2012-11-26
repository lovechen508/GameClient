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
// 函数：layout(bool resizable)
//
// 目的：进行元件自身及子元件的布局
//
void YCUITr::layout(bool resizable)
{
	// 检查 background
	buildBackground();
		
	int leftTopX = myMarginLeft + myPaddingLeft;
	int leftTopY = myMarginTop + myPaddingTop;

	//
	// 在布局过程中布局方式从左到右进行
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
// 函数：validateSubTag(const char* name)
//
// 目的：校验子控件
//
bool YCUITr::validateSubTag(const char* name)
{
	SMART_ASSERT(name != NULL);
	return (strcmp("td", name) == 0);
}