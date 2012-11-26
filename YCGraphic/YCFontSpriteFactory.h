#ifndef _INCLUDE_YCFONTSPRITEFACTORY_H_
#define _INCLUDE_YCFONTSPRITEFACTORY_H_

#include "YCUITagDef.h"
#include "YCGraphic.h"

class YCIFontSprite;

/*
 * YCFontSpriteFactory : FontSprite工厂类
 */
class YCFontSpriteFactory
{
public:

	YCFontSpriteFactory(YCGraphic::YCD3DSprite* sprite);
	
	~YCFontSpriteFactory(void);

	//
	// 函数：createFontSprite(E_FONT_TYPE type)
	//
	// 目的：工厂方法构造字符绘制精灵
	//
	YCIFontSprite * createFontSprite(E_FONT_TYPE type);

private:

	YCGraphic::YCD3DSprite* mySprite;
};

#endif

