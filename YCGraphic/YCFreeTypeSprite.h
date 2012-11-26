#ifndef _INCLUDE_YCFREETYPESPRITE_H_
#define _INCLUDE_YCFREETYPESPRITE_H_

#include "YCGraphic.h"
#include "YCUITagDef.h"
#include "YCIFontSprite.h"

class YCFreeTypeSpriteImpl;

/*
 * YCFreeTypeSprite : FreeType字符精灵实现类
 */
class YCFreeTypeSprite :
	public YCIFontSprite
{
public:

	//
	// 函数：createLibrary()
	//
	// 目的：获取FT_Library
	//
	static bool createLibrary();

	//
	// 函数：releaseLibrary()
	//
	// 目的：释放FT_Library
	//
	static void releaseLibrary();

public:

	YCFreeTypeSprite(YCGraphic::YCD3DSprite* sprite, E_FONT_TYPE type);

	virtual ~YCFreeTypeSprite(void);

	//
	// 函数：getFontWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height)
	//
	// 目的：取得特定字体大小下面高和宽
	//
	virtual bool getWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height);

	//
	// 函数：draw(const char* info, unsigned int left, unsigned int top)
	//
	// 目的：完成字符串的绘制
	//
	virtual void draw(const char* info, unsigned int left, unsigned int top);

private:

	//
	// 属性：绘制精灵，不持有生命周期
	//
	YCGraphic::YCD3DSprite* mySprite;

	//
	// 属性：
	//
	YCFreeTypeSpriteImpl * myImpl;

};

#endif

