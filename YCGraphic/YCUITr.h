#ifndef _INCLUDE_YCUITR_H_
#define _INCLUDE_YCUITR_H_

#include "YCUITag.h"

class YCUITr :
	public YCUITag
{
public:
	YCUITr(YCUITagMeta* tagMeta);

	virtual ~YCUITr(void);

	//
	// 函数：layout(bool resizable)
	//
	// 目的：进行元件自身及子元件的布局
	//
	virtual void layout(bool resizable);	

private:

	//
	// 函数：validateSubTag(const char* name)
	//
	// 目的：校验子控件
	//
	virtual bool validateSubTag(const char* name);
};

#endif

