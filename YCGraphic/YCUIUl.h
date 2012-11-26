#ifndef _INCLUDE_YCUIUL_H_
#define _INCLUDE_YCUIUL_H_

#include "YCUITag.h"
#include "YCIScrollableTag.h"

class YCUIUl 
	: public YCUITag
	, public YCIScrollableTag
{
public:

	YCUIUl(YCUITagMeta* tagMeta);

	virtual ~YCUIUl(void);

private:

	//
	// 函数：validateSubTag(const char* name)
	//
	// 目的：校验子控件
	//
	virtual bool validateSubTag(const char* name);
};

#endif
