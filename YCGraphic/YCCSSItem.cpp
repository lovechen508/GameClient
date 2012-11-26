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
// ������
//
// Ŀ�ģ�
//
E_UITAG_CSS YCCSSItem::name()
{
	return myName;
}

//
// ������
//
// Ŀ�ģ�
//
void YCCSSItem::setValue(const char* value)
{
	if (value == NULL)
	{
		throw YCException(2002, "YCCSSItem::setValueΪNULL");
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
		throw YCException(2002, "YCCSSItem::setValueδ֪����ֵ");
	}
}

//
// ������
//
// Ŀ�ģ�
//
int YCCSSItem::toInt()
{
	if (myValue.type() != typeid(int))
	{
		throw YCException(2002, "YCCSSItem::toInt����ֵ��Ϊint����");
	}
	return any_cast<int>(myValue);
}

//
// ������
//
// Ŀ�ģ�
//
std::string YCCSSItem::toString()
{
	if (myValue.type() != typeid(std::string))
	{
		throw YCException(2002, "YCCSSItem::toString����ֵ��Ϊ�ַ�������");
	}
	return any_cast<std::string>(myValue);
}

//
// ������
//
// Ŀ�ģ�
//
double YCCSSItem::toDouble()
{
	if (myValue.type() != typeid(double))
	{
		throw YCException(2002, "YCCSSItem::toDouble����ֵ��Ϊdouble����");
	}
	return any_cast<double>(myValue);
}

//
// ������
//
// Ŀ�ģ�
//
bool YCCSSItem::toBoolean()
{
	if (myValue.type() != typeid(bool))
	{
		throw YCException(2002, "YCCSSItem::toBoolean����ֵ��Ϊ��������");
	}
	return any_cast<bool>(myValue);
}
