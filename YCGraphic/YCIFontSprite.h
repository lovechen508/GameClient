#ifndef _INCLUDE_YCIFONTSPRITE_H_
#define _INCLUDE_YCIFONTSPRITE_H_

#include "YCUITagDef.h"

/*
 * YCIFontSprite : 字符绘制精灵接口类
 */ 
class YCIFontSprite
{
public:

	enum E_YCFONT_PROPERTY
	{
		FONT_SIZE		= 1,	//大小
		FONT_COLOR		= 2,	//颜色
		FONT_STOKER		= 3,	//描边
		FONT_UNDERLINE	= 4,	//下划
		FONT_VERTICAL   = 5,    //竖直
		FONT_SPACER     = 6,    //间距
	};

public:

	YCIFontSprite(E_FONT_TYPE type);

	virtual ~YCIFontSprite(void);

	//
	// 函数：initialize()
	//
	// 目的：初始化，或者draw结束后重置
	//
	YCIFontSprite* initialize();

	//
	// 函数：setProperty(E_YCFONT_PROPERTY property, unsigned int value)
	//
	// 目的：设置绘制属性
	//
	YCIFontSprite* setProperty(E_YCFONT_PROPERTY property, unsigned int value);

	//
	// 函数：getFontWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height)
	//
	// 目的：取得特定字体大小下面高和宽
	//
	virtual bool getWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height) = 0;

	//
	// 函数：draw(const char* info, unsigned int left, unsigned int top)
	//
	// 目的：完成字符串的绘制
	//
	virtual void draw(const char* info, unsigned int left, unsigned int top) = 0;

protected:

	const E_FONT_TYPE myType;

	unsigned int mySize;

	unsigned int myColor;

	bool myStoker;

	unsigned int myStokerColor;

	bool myUnderline;

	unsigned int myUnderlineColor;

	bool myVertical;

	unsigned int mySpacer;

};

#endif
