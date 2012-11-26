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
// ������initialize()
//
// Ŀ�ģ���ʼ��������draw����������
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
// ������setProperty(E_YCFONT_PROPERTY property, unsigned int value)
//
// Ŀ�ģ����û�������
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

