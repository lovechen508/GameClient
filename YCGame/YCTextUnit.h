#ifndef _INCLUDE_YCTEXTUNIT_H_
#define _INCLUDE_YCTEXTUNIT_H_

#include "YCIMessageUnit.h"

/*
 * YCTextUnit�������֣��ɽض�, \0����
 */ 
class YCTextUnit :
	public YCIMessageUnit
{
public:
	
	YCTextUnit(const char* content, unsigned int length);
	
	virtual ~YCTextUnit(void);

	void setColor(unsigned int color);

	void setUnderline(bool enabled);

private:

	//
	// ����
	//
	char* myContent;

	//
	// ���ݳ���
	//
	unsigned int myLength;

	//
	// ������ɫ
	//
	unsigned int myColor;

	//
	// �Ƿ��»���
	//
	bool myUnderline;
};

#endif

