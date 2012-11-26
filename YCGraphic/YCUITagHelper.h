#ifndef _INCLUDE_YCUITAGHELPER_H_
#define _INCLUDE_YCUITAGHELPER_H_

#include <map>
#include <string>

#include "YCUITagDef.h"

/*
 * YCUITagHelper : UI 标签静态帮助类
 */
class YCUITagHelper
{

public:

	//
	// 函数：YCUITagHelper& Instance()
	//
	// 目的：取得唯一实例
	//
	static YCUITagHelper& Instance(); 
	
	//
	// 函数：Key2Char(const char* input)
	//
	// 目的：字符串到VK值的映射
	//
	static char Key2Char(const char* input);

	//
	// 函数：getSpecialValue(const char* input)
	//
	// 目的：取得特殊字符串值
	//
	//       SCREEN_WIDTH
	//       SCREEN_HEIGHT
	//       HALF_SCREEN_WIDTH
	//       HALF_SCREEN_HEIGHT
	//       ...
	//
	//       如果没有对应的字符串值，将抛出异常
	//
	static int getSpecialValue(const char* input);

	//
	// 函数：getPropertyEnum(const char* name)
	//
	// 目的：由xml文件的属性字符串名称转换为枚举值
	//
	E_UITAG_CSS getPropertyEnum(const char* name) const;

	//
	// 函数：getCSSType(E_UITAG_CSS property)
	//
	// 目的：由css文件的属性字符串类型
	//
	E_UITAG_PROPERTY_TYPE getCSSType(E_UITAG_CSS property) const;

	//
	// 函数：getPropertyType(const char* name)
	//
	// 目的：取得该属性值的类型
	//
	E_UITAG_PROPERTY_TYPE getPropertyType(const char* name) const;

private:

	YCUITagHelper(void);

	~YCUITagHelper(void);

private:

	static YCUITagHelper gInstance;

	std::map<std::string, E_UITAG_CSS> myCSSProperties;
	
	std::map<E_UITAG_CSS, E_UITAG_PROPERTY_TYPE> myCSSPropertyType;

	std::map<std::string, E_UITAG_PROPERTY_TYPE> myPropertyType;
};

#endif