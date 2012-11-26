#include "YCUIButton.h"

YCUIButton::YCUIButton(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
{
}

YCUIButton::~YCUIButton()
{
}

//
// 函数：draw()
//
// 目的：在指定处绘制该UI元件
//
void YCUIButton::draw()
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
bool YCUIButton::validateSubTag(const char* name)
{
	return false;
}