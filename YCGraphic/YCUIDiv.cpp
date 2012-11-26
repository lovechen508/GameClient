#include "YCUIDiv.h"


YCUIDiv::YCUIDiv(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
{
}


YCUIDiv::~YCUIDiv(void)
{
}
/*
//
// 函数：draw()
//
// 目的：在指定处绘制该UI元件
//
void YCUIDiv::draw()
{

}
*/
//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// 函数：validateSubTag(const char* name)
//
// 目的：校验子控件
//
bool YCUIDiv::validateSubTag(const char* name)
{
	return true;
}