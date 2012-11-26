#ifndef _INCLUDE_YCUIP_H_
#define _INCLUDE_YCUIP_H_

#include "YCUITag.h"

class YCUIP :
	public YCUITag
{
public:

	YCUIP(YCUITagMeta* tagMeta);
	
	virtual ~YCUIP(void);

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	//bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:

	//
	// 函数：extraDraw()
	//
	// 目的：UI元件的自定义绘制
	//
	virtual void extraDraw();

	//
	// 函数：extraCSS()
	//
	// 目的：处理标签特定的CSS属性
	//
	virtual void extraCSS();

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

