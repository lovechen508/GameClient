#ifndef _INCLUDE_YCD3DXFONTSPRITE_H_
#define _INCLUDE_YCD3DXFONTSPRITE_H_

#include "YCIFontSprite.h"

struct ID3DXFont;

/*
 * YCD3DXFontSprite : ID3DXFONT包装类
 */
class YCD3DXFontSprite :
	public YCIFontSprite
{
public:

	YCD3DXFontSprite(E_FONT_TYPE type, int width, int height);
	
	virtual ~YCD3DXFontSprite(void);

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
	virtual void draw(const char* info, unsigned int left, unsigned int top) = 0;

private:

	ID3DXFont* myFont;
};

#endif

