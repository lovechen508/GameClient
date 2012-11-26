#ifndef _INCLUDE_YCUISPAN_H_
#define _INCLUDE_YCUISPAN_H_

#include "YCUITag.h"

class YCUISpan :
	public YCUITag
{
public:
	
	YCUISpan(YCUITagMeta* tagMeta);

	virtual ~YCUISpan(void);

	//
	// 函数：draw()
	//
	// 目的：在指定处绘制该UI元件
	//
	void draw();
	
private:

	//
	// 函数：validateSubTag(const char* name)
	//
	// 目的：校验子控件
	//
	virtual bool validateSubTag(const char* name);
};

#endif

