#ifndef _INCLUDE_YCUITEXTFIELD_H_
#define _INCLUDE_YCUITEXTFIELD_H_

#include "YCUITag.h"

/*
 *
 */
class YCUITextField
	: public YCUITag
{
public:

	YCUITextField(YCUITagMeta* tagMeta);

	virtual ~YCUITextField();

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