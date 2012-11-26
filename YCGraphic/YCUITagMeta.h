#ifndef _INCLUDE_YCUITAGMETA_H_
#define _INCLUDE_YCUITAGMETA_H_

#include <map>

#include "YCUITagDef.h"
#include "YCUITagHelper.h"

#include "YCBasic\YCIDumper.h"

/*
 * YCUITagMeta : 标签元数据信息，不可变类
 */
class YCUITagMeta : public YCIDumper
{
	
public:
	
	YCUITagMeta(const char* name);
	
	~YCUITagMeta(void);

	//
	// 函数：char* name() const
	//
	// 目的：标签名
	//
	const char* name() const;

	//
	// 函数：isCompleted() const
	//
	// 目的：标签是否完整
	//
	bool isCompleted() const;

	//
	// 函数：setBaseTagName(const char* baseTagName)
	//
	// 目的：设置标签基类名
	//
	void setBaseTagName(const char* baseTagName);

	//
	// 函数：getBaseTagName()
	//
	// 目的：获取标签基类名
	//
	const char * getBaseTagName();

	//
	// 函数：setBaseTag(YCUITagMeta* baseTag)
	//
	// 目的：设置标签基类
	//
	void setBaseTag(YCUITagMeta* baseTag);

	//
	// 函数：getBaseTag()
	//
	// 目的：获取标签基类
	//
	YCUITagMeta * getBaseTag();

	//
	// 函数：enable(E_UITAG_CSS property)
	//
	// 目的：设定支持CSS属性
	//
	void enableCSS(E_UITAG_CSS property);

	//
	// 函数：isSupported(E_UITAG_CSS property)
	//
	// 目的：是否支持该属性
	//
	bool isSupportedCSS(E_UITAG_CSS property) const;

	//
	// 函数：enableAttribute(const char* attribute)
	//
	// 目的：设定支持属性
	//
	void enableAttribute(const char* attribute);

	//
	// 函数：isSupportedAttribute(const char* attribute)
	//
	// 目的：是否支持该属性
	//
	bool isSupportedAttribute(const char* attribute) const;

	//
	// 函数：getValueType(E_UITAG_CSS property) const
	//
	// 目的：取得属性值类型
	//	
	E_UITAG_PROPERTY_TYPE getValueType(E_UITAG_CSS property) const;

	//
	// 函数：dump()
	//
	// 目的：dump类内部状态为Html片段
	//
	virtual const char* dump();

private:

	// 标签名
	const char* myName;

	// 父标签名
	const char* myBaseTagName;

	// 父标签
	YCUITagMeta* myBaseTag;

	// 标签支持的属性
	std::map<std::string, bool> mySupportedProperties;

	// 标签支持的CSS属性
	std::map<E_UITAG_CSS, bool> mySupportedCSS;
	
};

#endif