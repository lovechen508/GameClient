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
// 函数：layout(bool resizable)
//
// 目的：进行元件自身及子元件的布局
//
void YCUITable::layout(bool resizable)
{
	// 检查 background
	buildBackground();
		
	int leftTopX = myMarginLeft + myPaddingLeft;
	int leftTopY = myMarginTop + myPaddingTop;

	//
	// 在布局过程中
	// 按照一行一行的布局方式从上到下进行
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
// 函数：extraDraw()
//
// 目的：UI元件的自定义绘制
//
void YCUITable::extraDraw()
{

}

//
// 函数：extraCSS()
//
// 目的：处理标签特定的CSS属性
//
void YCUITable::extraCSS()
{

}

//
// 函数：validateSubTag(const char* name)
//
// 目的：校验子控件
//
bool YCUITable::validateSubTag(const char* name)
{
	SMART_ASSERT(name != NULL);
	return ((strcmp("tr", name) == 0) || (strcmp("th", name) == 0));
}
