#include "YCUIRadio.h"

YCUIRadio::YCUIRadio(YCUITagMeta* tagMeta)
	: YCIUIToggleButton(tagMeta)
{
}

YCUIRadio::~YCUIRadio(void)
{
}

//
// 函数：draw()
//
// 目的：在指定处绘制该UI元件
//
void YCUIRadio::draw()
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
bool YCUIRadio::validateSubTag(const char* name)
{
	return false;
}