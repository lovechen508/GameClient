#include "YCUITextField.h"

YCUITextField::YCUITextField(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
{
}


YCUITextField::~YCUITextField(void)
{
}

//
// 函数：draw()
//
// 目的：在指定处绘制该UI元件
//
void YCUITextField::draw()
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
bool YCUITextField::validateSubTag(const char* name)
{
	return false;
}