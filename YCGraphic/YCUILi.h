#ifndef _INCLUDE_YCUILI_H_
#define _INCLUDE_YCUILI_H_

#include "YCUITag.h"

class YCUILi :
	public YCUITag
{
public:
	
	YCUILi(YCUITagMeta* tagMeta);

	virtual ~YCUILi(void);

	//
	// 函数：draw()
	//
	// 目的：在指定处绘制该UI元件
	//
	//virtual void draw();

protected:

	//
	// 函数：extraCSS()
	//
	// 目的：处理标签特定的CSS属性
	//
	virtual void extraCSS();

	//
	// 函数：extraDraw()
	//
	// 目的：在指定处绘制该UI元件
	//
	virtual void extraDraw();
	
private:

	//
	// 函数：validateSubTag(const char* name)
	//
	// 目的：校验子控件
	//
	virtual bool validateSubTag(const char* name);

private:
		
	int myFontSize;

	unsigned int myColor;

	int myLetterSpacing;

	//
	// 属性：myStrTextureWidth，myStrTextureHeight
	//
	// 目的：字符串纹理宽和高
	//
	int myStrTextureWidth;
	int myStrTextureHeight;

	//
	// 属性：myFontWeight
	//
	// 目的：粗细
	//
	E_FONT_WEIGHT myFontWeight;

	//
	// 属性：myTextAlign
	//
	// 目的：对齐方式
	//
	E_TEXT_ALIGN myTextAlign;

	//
	// 属性：myFontType
	//
	// 目的：字体
	//
	E_FONT_TYPE myFontType;
};

#endif

