#ifndef _INCLUDE_YCUICHECKBOX_H_
#define _INCLUDE_YCUICHECKBOX_H_

#include "YCIUIToggleButton.h"

/*
 * YCCheckBox : 
 */
class YCUICheckBox
	: public YCIUIToggleButton
{
public:

	YCUICheckBox(YCUITagMeta* tagMeta);

	virtual ~YCUICheckBox();

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