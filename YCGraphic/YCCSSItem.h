#ifndef _INCLUDE_YCCSSITEM_H_
#define _INCLUDE_YCCSSITEM_H_

#include "YCUITagDef.h"

#include "YCBasic\YCAny.h"

/*
 * YCCSSItem : CSS���ӦCSS�ļ���һ������
 *             ֵ����
 */
class YCCSSItem
{
public:
	
	YCCSSItem(E_UITAG_CSS name);

	~YCCSSItem(void);

	//
	// ������
	//
	// Ŀ�ģ�
	//
	E_UITAG_CSS name();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	void setValue(const char* value);
	
	//
	// ������
	//
	// Ŀ�ģ�
	//
	int toInt();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	std::string toString();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	double toDouble();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	bool toBoolean();

private:

	E_UITAG_CSS myName;

	YCAny myValue;

};

#endif
