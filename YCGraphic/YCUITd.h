#ifndef _INCLUDE_YCUITD_H_
#define _INCLUDE_YCUITD_H_

#include "YCUITag.h"

class YCUITd :
	public YCUITag
{
public:

	YCUITd(YCUITagMeta* tagMeta);

	virtual ~YCUITd(void);

	//
	// 函数：setDelimiterEnable(bool enable)
	//
	// 目的：是否绘制分隔符
	//
	void setDelimiterEnable(bool enable);

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

	//
	// 函数：handleFont()
	//
	// 目的：处理字符
	//
	void handleFont();

	//
	// 函数：handleDelimiter()
	//
	// 目的：处理分隔符
	//
	void handleDelimiter();

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

	//
	// 属性：myDelimiterEnabled
	//
	// 目的：分隔符
	//
	bool myDelimiterEnabled;

	unsigned int myDelimiterWidth;

	unsigned int myDelimiterColor;
};

#endif

