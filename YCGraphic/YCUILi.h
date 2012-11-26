#ifndef _INCLUDE_YCUILI_H_
#define _INCLUDE_YCUILI_H_

#include "YCUITag.h"

class YCUILi :
	public YCUITag
{
public:
	
	YCUILi(YCUITagMeta* tagMeta);

	virtual ~YCUILi(void);

	//
	// ������draw()
	//
	// Ŀ�ģ���ָ�������Ƹ�UIԪ��
	//
	//virtual void draw();

protected:

	//
	// ������extraCSS()
	//
	// Ŀ�ģ������ǩ�ض���CSS����
	//
	virtual void extraCSS();

	//
	// ������extraDraw()
	//
	// Ŀ�ģ���ָ�������Ƹ�UIԪ��
	//
	virtual void extraDraw();
	
private:

	//
	// ������validateSubTag(const char* name)
	//
	// Ŀ�ģ�У���ӿؼ�
	//
	virtual bool validateSubTag(const char* name);

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
};

#endif

