#ifndef _INCLUDE_YCCSSITEM_H_
#define _INCLUDE_YCCSSITEM_H_

#include "YCUITagDef.h"

#include "YCBasic\YCAny.h"

/*
 * YCCSSItem : CSS项，对应CSS文件的一行属性
 *             值对象
 */
class YCCSSItem
{
public:
	
	YCCSSItem(E_UITAG_CSS name);

	~YCCSSItem(void);

	//
	// 函数：
	//
	// 目的：
	//
	E_UITAG_CSS name();

	//
	// 函数：
	//
	// 目的：
	//
	void setValue(const char* value);
	
	//
	// 函数：
	//
	// 目的：
	//
	int toInt();

	//
	// 函数：
	//
	// 目的：
	//
	std::string toString();

	//
	// 函数：
	//
	// 目的：
	//
	double toDouble();

	//
	// 函数：
	//
	// 目的：
	//
	bool toBoolean();

private:

	E_UITAG_CSS myName;

	YCAny myValue;

};

#endif
