#ifndef _INCLUDE_YCUISPRITE_H_
#define _INCLUDE_YCUISPRITE_H_

#include "YCUIImg.h"

class YCUISprite :
	public YCUIImg
{
public:
	YCUISprite(YCUITagMeta* tagMeta);

	virtual ~YCUISprite(void);

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

