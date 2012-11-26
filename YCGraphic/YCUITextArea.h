#ifndef _INCLUDE_YCUITEXTAREA_H_
#define _INCLUDE_YCUITEXTAREA_H_

#include "YCUITextField.h"
#include "YCIScrollableTag.h"

/*
 *
 */
class YCUITextArea
	: public YCUITextField
	, public YCIScrollableTag
{
public:

	YCUITextArea(YCUITagMeta* tagMeta);

	virtual ~YCUITextArea();

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