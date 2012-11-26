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
// ������char* name() const
//
// Ŀ�ģ���ǩ��
//
const char* YCUITagMeta::name() const
{
	return myName;
}

//
// ������isCompleted() const
//
// Ŀ�ģ���ǩ�Ƿ�����
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
// ������setBaseTagName(const char* baseTagName)
//
// Ŀ�ģ����ñ�ǩ������
//
void YCUITagMeta::setBaseTagName(const char* baseTagName)
{
	myBaseTagName = baseTagName;
}

//
// ������getBaseTagName()
//
// Ŀ�ģ���ȡ��ǩ������
//
const char * YCUITagMeta::getBaseTagName()
{
	return myBaseTagName;
}

//
// ������setBaseTag(YCUITagMeta* baseTag)
//
// Ŀ�ģ�
//
void YCUITagMeta::setBaseTag(YCUITagMeta* baseTag)
{
	myBaseTag = baseTag;
}

//
// ������getBaseTag()
//
// Ŀ�ģ���ȡ��ǩ����
//
YCUITagMeta * YCUITagMeta::getBaseTag()
{
	return myBaseTag;
}

//
// ������enableCSS(E_UITAG_CSS property)
//
// Ŀ�ģ��趨֧������
//
void YCUITagMeta::enableCSS(E_UITAG_CSS property)
{
	if (property == UITAG_INVALID)
	{
		throw YCException(2002, "YCUITagMeta::enable������Ч����ֵ");
	}

	std::map<E_UITAG_CSS, bool>::iterator at = mySupportedCSS.find(property);
	if (at == mySupportedCSS.end())
	{
		mySupportedCSS[property] = true;
	}
}

//
// ������isSupported(E_UITAG_CSS property)
//
// Ŀ�ģ��Ƿ�֧�ָ�����
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
// ������enableAttribute(const char* attribute)
//
// Ŀ�ģ��趨֧������
//
void YCUITagMeta::enableAttribute(const char* attribute)
{
	if (attribute == NULL)
	{
		throw YCException(2002, "YCUITagMeta::enableAttribute������Ч����ֵ");
	}

	std::map<std::string, bool>::iterator at = mySupportedProperties.find(attribute);
	if (at == mySupportedProperties.end())
	{
		mySupportedProperties[attribute] = true;
	}
}

//
// ������isSupportedAttribute(const char* attribute)
//
// Ŀ�ģ��Ƿ�֧�ָ�����
//
bool YCUITagMeta::isSupportedAttribute(const char* attribute) const
{
	if (attribute == NULL)
	{
		throw YCException(2002, "YCUITagMeta::enableAttribute������Ч����ֵ");
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
// ������getValueType(E_UITAG_CSS property) const
//
// Ŀ�ģ�ȡ������ֵ����
//	
E_UITAG_PROPERTY_TYPE YCUITagMeta::getValueType(E_UITAG_CSS property) const
{
	if (!isSupportedCSS(property))
	{
		throw YCException(2002, "YCUITagMeta::getValueType��֧�ֵ�����ֵ���ͣ�");
	}

	return TAG_STRING;
}

//
// ������dump()
//
// Ŀ�ģ�dump���ڲ�״̬ΪHtmlƬ��
//
const char* YCUITagMeta::dump()
{
	return NULL;
}
