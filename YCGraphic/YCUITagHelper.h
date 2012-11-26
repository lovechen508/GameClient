#ifndef _INCLUDE_YCUITAGHELPER_H_
#define _INCLUDE_YCUITAGHELPER_H_

#include <map>
#include <string>

#include "YCUITagDef.h"

/*
 * YCUITagHelper : UI ��ǩ��̬������
 */
class YCUITagHelper
{

public:

	//
	// ������YCUITagHelper& Instance()
	//
	// Ŀ�ģ�ȡ��Ψһʵ��
	//
	static YCUITagHelper& Instance(); 
	
	//
	// ������Key2Char(const char* input)
	//
	// Ŀ�ģ��ַ�����VKֵ��ӳ��
	//
	static char Key2Char(const char* input);

	//
	// ������getSpecialValue(const char* input)
	//
	// Ŀ�ģ�ȡ�������ַ���ֵ
	//
	//       SCREEN_WIDTH
	//       SCREEN_HEIGHT
	//       HALF_SCREEN_WIDTH
	//       HALF_SCREEN_HEIGHT
	//       ...
	//
	//       ���û�ж�Ӧ���ַ���ֵ�����׳��쳣
	//
	static int getSpecialValue(const char* input);

	//
	// ������getPropertyEnum(const char* name)
	//
	// Ŀ�ģ���xml�ļ��������ַ�������ת��Ϊö��ֵ
	//
	E_UITAG_CSS getPropertyEnum(const char* name) const;

	//
	// ������getCSSType(E_UITAG_CSS property)
	//
	// Ŀ�ģ���css�ļ��������ַ�������
	//
	E_UITAG_PROPERTY_TYPE getCSSType(E_UITAG_CSS property) const;

	//
	// ������getPropertyType(const char* name)
	//
	// Ŀ�ģ�ȡ�ø�����ֵ������
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