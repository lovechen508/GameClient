#include "YCUITagMeta.h"

#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"

YCUITagMeta::YCUITagMeta(const char* name)
	: myName(name)
	, myBaseTagName(NULL)
	, myBaseTag(NULL)
{
}


YCUITagMeta::~YCUITagMeta(void)
{
	delete myName;
	delete myBaseTagName;
}

//
// 函数：char* name() const
//
// 目的：标签名
//
const char* YCUITagMeta::name() const
{
	return myName;
}

//
// 函数：isCompleted() const
//
// 目的：标签是否完整
//
bool YCUITagMeta::isCompleted() const
{
	if (myBaseTagName != NULL)
	{
		return (myBaseTag != NULL);
	}
	return true;
}

//
// 函数：setBaseTagName(const char* baseTagName)
//
// 目的：设置标签基类名
//
void YCUITagMeta::setBaseTagName(const char* baseTagName)
{
	myBaseTagName = baseTagName;
}

//
// 函数：getBaseTagName()
//
// 目的：获取标签基类名
//
const char * YCUITagMeta::getBaseTagName()
{
	return myBaseTagName;
}

//
// 函数：setBaseTag(YCUITagMeta* baseTag)
//
// 目的：
//
void YCUITagMeta::setBaseTag(YCUITagMeta* baseTag)
{
	myBaseTag = baseTag;
}

//
// 函数：getBaseTag()
//
// 目的：获取标签基类
//
YCUITagMeta * YCUITagMeta::getBaseTag()
{
	return myBaseTag;
}

//
// 函数：enableCSS(E_UITAG_CSS property)
//
// 目的：设定支持属性
//
void YCUITagMeta::enableCSS(E_UITAG_CSS property)
{
	if (property == UITAG_INVALID)
	{
		throw YCException(2002, "YCUITagMeta::enable设置无效属性值");
	}

	std::map<E_UITAG_CSS, bool>::iterator at = mySupportedCSS.find(property);
	if (at == mySupportedCSS.end())
	{
		mySupportedCSS[property] = true;
	}
}

//
// 函数：isSupported(E_UITAG_CSS property)
//
// 目的：是否支持该属性
//
bool YCUITagMeta::isSupportedCSS(E_UITAG_CSS property) const
{
	std::map<E_UITAG_CSS, bool>::const_iterator at = mySupportedCSS.find(property);
	if (at == mySupportedCSS.end())
	{
		return (myBaseTag != NULL) 
			 ? myBaseTag->isSupportedCSS(property)
			 : false;
	}
	return true;
}

//
// 函数：enableAttribute(const char* attribute)
//
// 目的：设定支持属性
//
void YCUITagMeta::enableAttribute(const char* attribute)
{
	if (attribute == NULL)
	{
		throw YCException(2002, "YCUITagMeta::enableAttribute设置无效属性值");
	}

	std::map<std::string, bool>::iterator at = mySupportedProperties.find(attribute);
	if (at == mySupportedProperties.end())
	{
		mySupportedProperties[attribute] = true;
	}
}

//
// 函数：isSupportedAttribute(const char* attribute)
//
// 目的：是否支持该属性
//
bool YCUITagMeta::isSupportedAttribute(const char* attribute) const
{
	if (attribute == NULL)
	{
		throw YCException(2002, "YCUITagMeta::enableAttribute设置无效属性值");
	}

	std::map<std::string, bool>::const_iterator at = mySupportedProperties.find(attribute);
	if (at == mySupportedProperties.end())
	{
		return (myBaseTag != NULL) 
			 ? myBaseTag->isSupportedAttribute(attribute)
			 : false;
	}
	return true;
}

//
// 函数：getValueType(E_UITAG_CSS property) const
//
// 目的：取得属性值类型
//	
E_UITAG_PROPERTY_TYPE YCUITagMeta::getValueType(E_UITAG_CSS property) const
{
	if (!isSupportedCSS(property))
	{
		throw YCException(2002, "YCUITagMeta::getValueType不支持的属性值类型！");
	}

	return TAG_STRING;
}

//
// 函数：dump()
//
// 目的：dump类内部状态为Html片段
//
const char* YCUITagMeta::dump()
{
	return NULL;
}
