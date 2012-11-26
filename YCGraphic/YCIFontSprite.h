#ifndef _INCLUDE_YCIFONTSPRITE_H_
#define _INCLUDE_YCIFONTSPRITE_H_

#include "YCUITagDef.h"

/*
 * YCIFontSprite : �ַ����ƾ���ӿ���
 */ 
class YCIFontSprite
{
public:

	enum E_YCFONT_PROPERTY
	{
		FONT_SIZE		= 1,	//��С
		FONT_COLOR		= 2,	//��ɫ
		FONT_STOKER		= 3,	//���
		FONT_UNDERLINE	= 4,	//�»�
		FONT_VERTICAL   = 5,    //��ֱ
		FONT_SPACER     = 6,    //���
	};

public:

	YCIFontSprite(E_FONT_TYPE type);

	virtual ~YCIFontSprite(void);

	//
	// ������initialize()
	//
	// Ŀ�ģ���ʼ��������draw����������
	//
	YCIFontSprite* initialize();

	//
	// ������setProperty(E_YCFONT_PROPERTY property, unsigned int value)
	//
	// Ŀ�ģ����û�������
	//
	YCIFontSprite* setProperty(E_YCFONT_PROPERTY property, unsigned int value);

	//
	// ������getFontWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height)
	//
	// Ŀ�ģ�ȡ���ض������С����ߺͿ�
	//
	virtual bool getWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height) = 0;

	//
	// ������draw(const char* info, unsigned int left, unsigned int top)
	//
	// Ŀ�ģ�����ַ����Ļ���
	//
	virtual void draw(const char* info, unsigned int left, unsigned int top) = 0;

protected:

	const E_FONT_TYPE myType;

	unsigned int mySize;

	unsigned int myColor;

	bool myStoker;

	unsigned int myStokerColor;

	bool myUnderline;

	unsigned int myUnderlineColor;

	bool myVertical;

	unsigned int mySpacer;

};

#endif
