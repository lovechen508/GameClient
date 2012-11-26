#ifndef _INCLUDE_YCUIRADIO_H_
#define _INCLUDE_YCUIRADIO_H_

#include "YCIUIToggleButton.h"

class YCUIRadio :
	public YCIUIToggleButton
{
public:

	YCUIRadio(YCUITagMeta* tagMeta);

	virtual ~YCUIRadio(void);

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
