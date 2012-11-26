#include "YCIFontSprite.h"

YCIFontSprite::YCIFontSprite(E_FONT_TYPE type)
	: myType(type)
{
	initialize();
}

YCIFontSprite::~YCIFontSprite(void)
{
}

//
// 函数：initialize()
//
// 目的：初始化，或者draw结束后重置
//
YCIFontSprite* YCIFontSprite::initialize()
{
	mySize = 12;
	myColor = 0x000000;
	myStoker = false;
	myUnderline = false;
	myVertical = false;
	mySpacer = 0;

	return this;
}

//
// 函数：setProperty(E_YCFONT_PROPERTY property, unsigned int value)
//
// 目的：设置绘制属性
//
YCIFontSprite* YCIFontSprite::setProperty(E_YCFONT_PROPERTY property, unsigned int value)
{
	if      (property == FONT_SIZE)      { mySize = value;}
	else if (property == FONT_COLOR)     { myColor = value;}
	else if (property == FONT_STOKER)    { myStoker = true; myStokerColor = value;}
	else if (property == FONT_UNDERLINE) { myUnderline = true; myUnderlineColor = value;}
	else if (property == FONT_VERTICAL)  { myVertical = value == 1 ? true : false;}
	else if (property == FONT_SPACER)    { mySpacer = value;}

	return this;
}

