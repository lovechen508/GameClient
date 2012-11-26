#include "YCUISpan.h"


YCUISpan::YCUISpan(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
{
}


YCUISpan::~YCUISpan(void)
{
}

//
// 函数：draw()
//
// 目的：在指定处绘制该UI元件
//
void YCUISpan::draw()
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
bool YCUISpan::validateSubTag(const char* name)
{
	return false;
}
