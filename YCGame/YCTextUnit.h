#ifndef _INCLUDE_YCTEXTUNIT_H_
#define _INCLUDE_YCTEXTUNIT_H_

#include "YCIMessageUnit.h"

/*
 * YCTextUnit：纯文字，可截断, \0结束
 */ 
class YCTextUnit :
	public YCIMessageUnit
{
public:
	
	YCTextUnit(const char* content, unsigned int length);
	
	virtual ~YCTextUnit(void);

	void setColor(unsigned int color);

	void setUnderline(bool enabled);

private:

	//
	// 内容
	//
	char* myContent;

	//
	// 内容长度
	//
	unsigned int myLength;

	//
	// 字体颜色
	//
	unsigned int myColor;

	//
	// 是否下划线
	//
	bool myUnderline;
};

#endif

