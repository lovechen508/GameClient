#include "YCUIImg.h"


YCUIImg::YCUIImg(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
{
}


YCUIImg::~YCUIImg(void)
{
}

//
// 函数：draw()
//
// 目的：在指定处绘制该UI元件
//
void YCUIImg::draw()
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
bool YCUIImg::validateSubTag(const char* name)
{
	return false;
}
