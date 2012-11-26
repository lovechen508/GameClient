#ifndef _INCLUDE_YCUITAGMETA_H_
#define _INCLUDE_YCUITAGMETA_H_

#include <map>

#include "YCUITagDef.h"
#include "YCUITagHelper.h"

#include "YCBasic\YCIDumper.h"

/*
 * YCUITagMeta : ��ǩԪ������Ϣ�����ɱ���
 */
class YCUITagMeta : public YCIDumper
{
	
public:
	
	YCUITagMeta(const char* name);
	
	~YCUITagMeta(void);

	//
	// ������char* name() const
	//
	// Ŀ�ģ���ǩ��
	//
	const char* name() const;

	//
	// ������isCompleted() const
	//
	// Ŀ�ģ���ǩ�Ƿ�����
	//
	bool isCompleted() const;

	//
	// ������setBaseTagName(const char* baseTagName)
	//
	// Ŀ�ģ����ñ�ǩ������
	//
	void setBaseTagName(const char* baseTagName);

	//
	// ������getBaseTagName()
	//
	// Ŀ�ģ���ȡ��ǩ������
	//
	const char * getBaseTagName();

	//
	// ������setBaseTag(YCUITagMeta* baseTag)
	//
	// Ŀ�ģ����ñ�ǩ����
	//
	void setBaseTag(YCUITagMeta* baseTag);

	//
	// ������getBaseTag()
	//
	// Ŀ�ģ���ȡ��ǩ����
	//
	YCUITagMeta * getBaseTag();

	//
	// ������enable(E_UITAG_CSS property)
	//
	// Ŀ�ģ��趨֧��CSS����
	//
	void enableCSS(E_UITAG_CSS property);

	//
	// ������isSupported(E_UITAG_CSS property)
	//
	// Ŀ�ģ��Ƿ�֧�ָ�����
	//
	bool isSupportedCSS(E_UITAG_CSS property) const;

	//
	// ������enableAttribute(const char* attribute)
	//
	// Ŀ�ģ��趨֧������
	//
	void enableAttribute(const char* attribute);

	//
	// ������isSupportedAttribute(const char* attribute)
	//
	// Ŀ�ģ��Ƿ�֧�ָ�����
	//
	bool isSupportedAttribute(const char* attribute) const;

	//
	// ������getValueType(E_UITAG_CSS property) const
	//
	// Ŀ�ģ�ȡ������ֵ����
	//	
	E_UITAG_PROPERTY_TYPE getValueType(E_UITAG_CSS property) const;

	//
	// ������dump()
	//
	// Ŀ�ģ�dump���ڲ�״̬ΪHtmlƬ��
	//
	virtual const char* dump();

private:

	// ��ǩ��
	const char* myName;

	// ����ǩ��
	const char* myBaseTagName;

	// ����ǩ
	YCUITagMeta* myBaseTag;

	// ��ǩ֧�ֵ�����
	std::map<std::string, bool> mySupportedProperties;

	// ��ǩ֧�ֵ�CSS����
	std::map<E_UITAG_CSS, bool> mySupportedCSS;
	
};

#endif