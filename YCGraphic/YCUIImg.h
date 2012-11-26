#ifndef _INCLUDE_YCUIIMG_H_
#define _INCLUDE_YCUIIMG_H_

#include "YCUITag.h"

class YCUIImg :
	public YCUITag
{
public:
	
	YCUIImg(YCUITagMeta* tagMeta);
	
	virtual ~YCUIImg(void);

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

