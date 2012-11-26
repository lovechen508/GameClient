#ifndef _INCLUDE_YCUITD_H_
#define _INCLUDE_YCUITD_H_

#include "YCUITag.h"

class YCUITd :
	public YCUITag
{
public:

	YCUITd(YCUITagMeta* tagMeta);

	virtual ~YCUITd(void);

	//
	// ������setDelimiterEnable(bool enable)
	//
	// Ŀ�ģ��Ƿ���Ʒָ���
	//
	void setDelimiterEnable(bool enable);

	//
	// ������extraDraw()
	//
	// Ŀ�ģ�UIԪ�����Զ������
	//
	virtual void extraDraw();
		
	//
	// ������extraCSS()
	//
	// Ŀ�ģ������ǩ�ض���CSS����
	//
	virtual void extraCSS();

private:

	//
	// ������validateSubTag(const char* name)
	//
	// Ŀ�ģ�У���ӿؼ�
	//
	virtual bool validateSubTag(const char* name);

	//
	// ������handleFont()
	//
	// Ŀ�ģ������ַ�
	//
	void handleFont();

	//
	// ������handleDelimiter()
	//
	// Ŀ�ģ�����ָ���
	//
	void handleDelimiter();

private:

	int myFontSize;

	unsigned int myColor;

	int myLetterSpacing;
	
	//
	// ���ԣ�myStrTextureWidth��myStrTextureHeight
	//
	// Ŀ�ģ��ַ��������͸�
	//
	int myStrTextureWidth;
	int myStrTextureHeight;

	//
	// ���ԣ�myFontWeight
	//
	// Ŀ�ģ���ϸ
	//
	E_FONT_WEIGHT myFontWeight;

	//
	// ���ԣ�myTextAlign
	//
	// Ŀ�ģ����뷽ʽ
	//
	E_TEXT_ALIGN myTextAlign;

	//
	// ���ԣ�myFontType
	//
	// Ŀ�ģ�����
	//
	E_FONT_TYPE myFontType;

	//
	// ���ԣ�myDelimiterEnabled
	//
	// Ŀ�ģ��ָ���
	//
	bool myDelimiterEnabled;

	unsigned int myDelimiterWidth;

	unsigned int myDelimiterColor;
};

#endif

