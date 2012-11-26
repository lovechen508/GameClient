#ifndef _INCLUDE_YCD3DXFONTSPRITE_H_
#define _INCLUDE_YCD3DXFONTSPRITE_H_

#include "YCIFontSprite.h"

struct ID3DXFont;

/*
 * YCD3DXFontSprite : ID3DXFONT��װ��
 */
class YCD3DXFontSprite :
	public YCIFontSprite
{
public:

	YCD3DXFontSprite(E_FONT_TYPE type, int width, int height);
	
	virtual ~YCD3DXFontSprite(void);

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
	virtual void draw(const char* info, unsigned int left, unsigned int top) = 0;

private:

	ID3DXFont* myFont;
};

#endif

