#ifndef _INCLUDE_YCUITEXTFIELD_H_
#define _INCLUDE_YCUITEXTFIELD_H_

#include "YCUITag.h"

/*
 *
 */
class YCUITextField
	: public YCUITag
{
public:

	YCUITextField(YCUITagMeta* tagMeta);

	virtual ~YCUITextField();

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