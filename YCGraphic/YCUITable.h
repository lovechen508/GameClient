#ifndef _INCLUDE_YCUITABLE_H_
#define _INCLUDE_YCUITABLE_H_

#include "YCUITag.h"
#include "YCIScrollableTag.h"

class YCUITable 
	: public YCUITag
	, public YCIScrollableTag
{
public:

	YCUITable(YCUITagMeta* tagMeta);

	virtual ~YCUITable(void);

	//
	// 函数：layout(bool resizable)
	//
	// 目的：进行元件自身及子元件的布局
	//
	virtual void layout(bool resizable);

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
};

#endif

