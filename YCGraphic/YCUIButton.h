#ifndef _INCLUDE_YCUIBUTTON_H_
#define _INCLUDE_YCUIBUTTON_H_

#include "YCUITag.h"

class YCUIButton :
	public YCUITag
{
public:

	YCUIButton(YCUITagMeta* tagMeta);

	virtual ~YCUIButton(void);

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
