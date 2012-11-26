#ifndef _INCLUDE_YCFONTSPRITEFACTORY_H_
#define _INCLUDE_YCFONTSPRITEFACTORY_H_

#include "YCUITagDef.h"
#include "YCGraphic.h"

class YCIFontSprite;

/*
 * YCFontSpriteFactory : FontSprite������
 */
class YCFontSpriteFactory
{
public:

	YCFontSpriteFactory(YCGraphic::YCD3DSprite* sprite);
	
	~YCFontSpriteFactory(void);

	//
	// ������createFontSprite(E_FONT_TYPE type)
	//
	// Ŀ�ģ��������������ַ����ƾ���
	//
	YCIFontSprite * createFontSprite(E_FONT_TYPE type);

private:

	YCGraphic::YCD3DSprite* mySprite;
};

#endif

