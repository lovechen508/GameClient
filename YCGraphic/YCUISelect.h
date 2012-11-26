#ifndef _INCLUDE_YCUISELECT_H_
#define _INCLUDE_YCUISELECT_H_

#include "YCUITag.h"

/*
 * YCSelect :  下拉式菜单
 */
class YCUISelect 
	: public YCUITag
{
public:

	YCUISelect(YCUITagMeta* tagMeta);
	
	virtual ~YCUISelect(void);

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