#include "YCUISprite.h"


YCUISprite::YCUISprite(YCUITagMeta* tagMeta)
	: YCUIImg(tagMeta)
{
}


YCUISprite::~YCUISprite(void)
{
}

//
// 函数：draw()
//
// 目的：在指定处绘制该UI元件
//
void YCUISprite::draw()
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
bool YCUISprite::validateSubTag(const char* name)
{
	return false;
}
