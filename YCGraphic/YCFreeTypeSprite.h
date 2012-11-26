#ifndef _INCLUDE_YCFREETYPESPRITE_H_
#define _INCLUDE_YCFREETYPESPRITE_H_

#include "YCGraphic.h"
#include "YCUITagDef.h"
#include "YCIFontSprite.h"

class YCFreeTypeSpriteImpl;

/*
 * YCFreeTypeSprite : FreeType�ַ�����ʵ����
 */
class YCFreeTypeSprite :
	public YCIFontSprite
{
public:

	//
	// ������createLibrary()
	//
	// Ŀ�ģ���ȡFT_Library
	//
	static bool createLibrary();

	//
	// ������releaseLibrary()
	//
	// Ŀ�ģ��ͷ�FT_Library
	//
	static void releaseLibrary();

public:

	YCFreeTypeSprite(YCGraphic::YCD3DSprite* sprite, E_FONT_TYPE type);

	virtual ~YCFreeTypeSprite(void);

	//
	// ������getFontWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height)
	//
	// Ŀ�ģ�ȡ���ض������С����ߺͿ�
	//
	virtual bool getWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height);

	//
	// ������draw(const char* info, unsigned int left, unsigned int top)
	//
	// Ŀ�ģ�����ַ����Ļ���
	//
	virtual void draw(const char* info, unsigned int left, unsigned int top);

private:

	//
	// ���ԣ����ƾ��飬��������������
	//
	YCGraphic::YCD3DSprite* mySprite;

	//
	// ���ԣ�
	//
	YCFreeTypeSpriteImpl * myImpl;

};

#endif

