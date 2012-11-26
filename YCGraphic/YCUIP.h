#ifndef _INCLUDE_YCUIP_H_
#define _INCLUDE_YCUIP_H_

#include "YCUITag.h"

class YCUIP :
	public YCUITag
{
public:

	YCUIP(YCUITagMeta* tagMeta);
	
	virtual ~YCUIP(void);

	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	//bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:

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

