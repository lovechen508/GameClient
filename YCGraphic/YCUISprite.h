#ifndef _INCLUDE_YCUISPRITE_H_
#define _INCLUDE_YCUISPRITE_H_

#include "YCUIImg.h"

class YCUISprite :
	public YCUIImg
{
public:
	YCUISprite(YCUITagMeta* tagMeta);

	virtual ~YCUISprite(void);

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

