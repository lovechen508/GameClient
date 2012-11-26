#include "YCUITextArea.h"

YCUITextArea::YCUITextArea(YCUITagMeta* tagMeta)
	: YCUITextField(tagMeta)
{
}


YCUITextArea::~YCUITextArea(void)
{
}

//
// 函数：draw()
//
// 目的：在指定处绘制该UI元件
//
void YCUITextArea::draw()
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
bool YCUITextArea::validateSubTag(const char* name)
{
	return false;
}