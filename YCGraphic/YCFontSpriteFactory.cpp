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
// ������createFontSprite(E_FONT_TYPE type)
//
// Ŀ�ģ��������������ַ����ƾ���
//
YCIFontSprite * YCFontSpriteFactory::createFontSprite(E_FONT_TYPE type)
{
	return new YCFreeTypeSprite(mySprite, type);
}
