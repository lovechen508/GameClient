#include "YCCSSItem.h"

#include "YCBasic\YCAssert.h"

#include "YCUITagHelper.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"

YCCSSItem::YCCSSItem(E_UITAG_CSS name)
	: myName(name)
{
}

YCCSSItem::~YCCSSItem(void)
{
}

//
// 函数：
//
// 目的：
//
E_UITAG_CSS YCCSSItem::name()
{
	return myName;
}

//
// 函数：
//
// 目的：
//
void YCCSSItem::setValue(const char* value)
{
	if (value == NULL)
	{
		throw YCException(2002, "YCCSSItem::setValue为NULL");
	}

	E_UITAG_PROPERTY_TYPE propertyType = YCUITagHelper::Instance().getCSSType(myName);
	if      (propertyType == TAG_INT)
	{
		myValue = atoi(value);
	}
	else if (propertyType == TAG_DOUBLE)
	{
		myValue = atof(value);
	}
	else if (propertyType == TAG_STRING)
	{
		myValue = std::string(value);
	}
	else if (propertyType == TAG_BOOLEAN)
	{
		if      ( (strcmp(value, "true") == 0) || (strcmp(value, "1") == 0) )
		{
			myValue = true;
		}
		else if ( (strcmp(value, "false") == 0) || (strcmp(value, "0") == 0) )
		{
			myValue = false;
		}
	}
	else
	{
		throw YCException(2002, "YCCSSItem::setValue未知属性值");
	}
}

//
// 函数：
//
// 目的：
//
int YCCSSItem::toInt()
{
	if (myValue.type() != typeid(int))
	{
		throw YCException(2002, "YCCSSItem::toInt属性值不为int类型");
	}
	return any_cast<int>(myValue);
}

//
// 函数：
//
// 目的：
//
std::string YCCSSItem::toString()
{
	if (myValue.type() != typeid(std::string))
	{
		throw YCException(2002, "YCCSSItem::toString属性值不为字符串类型");
	}
	return any_cast<std::string>(myValue);
}

//
// 函数：
//
// 目的：
//
double YCCSSItem::toDouble()
{
	if (myValue.type() != typeid(double))
	{
		throw YCException(2002, "YCCSSItem::toDouble属性值不为double类型");
	}
	return any_cast<double>(myValue);
}

//
// 函数：
//
// 目的：
//
bool YCCSSItem::toBoolean()
{
	if (myValue.type() != typeid(bool))
	{
		throw YCException(2002, "YCCSSItem::toBoolean属性值不为布尔类型");
	}
	return any_cast<bool>(myValue);
}
