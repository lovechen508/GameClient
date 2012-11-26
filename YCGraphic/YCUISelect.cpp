#include "YCUISelect.h"

YCUISelect::YCUISelect(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
{
}


YCUISelect::~YCUISelect(void)
{
}

//
// 函数：draw()
//
// 目的：在指定处绘制该UI元件
//
void YCUISelect::draw()
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
bool YCUISelect::validateSubTag(const char* name)
{
	return false;
}