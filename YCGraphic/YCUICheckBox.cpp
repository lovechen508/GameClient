#include "YCUICheckBox.h"

YCUICheckBox::YCUICheckBox(YCUITagMeta* tagMeta)
	: YCIUIToggleButton(tagMeta)
{
}


YCUICheckBox::~YCUICheckBox(void)
{
}

//
// 函数：draw()
//
// 目的：在指定处绘制该UI元件
//
void YCUICheckBox::draw()
{

}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// 函数：validateSubTag(const char* name)
//
// 目的：校验子控件
//
bool YCUICheckBox::validateSubTag(const char* name)
{
	return false;
}