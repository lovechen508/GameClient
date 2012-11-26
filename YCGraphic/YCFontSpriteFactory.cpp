#include "YCFontSpriteFactory.h"

#include "YCIFontSprite.h"
#include "YCD3DXFontSprite.h"
#include "YCFreeTypeSprite.h"

YCFontSpriteFactory::YCFontSpriteFactory(YCGraphic::YCD3DSprite* sprite)
	: mySprite(sprite)
{
	YCFreeTypeSprite::createLibrary();
}


YCFontSpriteFactory::~YCFontSpriteFactory(void)
{
	YCFreeTypeSprite::releaseLibrary();
}

//
// 函数：createFontSprite(E_FONT_TYPE type)
//
// 目的：工厂方法构造字符绘制精灵
//
YCIFontSprite * YCFontSpriteFactory::createFontSprite(E_FONT_TYPE type)
{
	return new YCFreeTypeSprite(mySprite, type);
}
