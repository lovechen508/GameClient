#ifndef _INCLUDE_YCUISPAN_H_
#define _INCLUDE_YCUISPAN_H_

#include "YCUITag.h"

class YCUISpan :
	public YCUITag
{
public:
	
	YCUISpan(YCUITagMeta* tagMeta);

	virtual ~YCUISpan(void);

	//
	// ������draw()
	//
	// Ŀ�ģ���ָ�������Ƹ�UIԪ��
	//
	void draw();
	
private:

	//
	// ������validateSubTag(const char* name)
	//
	// Ŀ�ģ�У���ӿؼ�
	//
	virtual bool validateSubTag(const char* name);
};

#endif

