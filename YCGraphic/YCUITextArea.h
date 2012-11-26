#ifndef _INCLUDE_YCUITEXTAREA_H_
#define _INCLUDE_YCUITEXTAREA_H_

#include "YCUITextField.h"
#include "YCIScrollableTag.h"

/*
 *
 */
class YCUITextArea
	: public YCUITextField
	, public YCIScrollableTag
{
public:

	YCUITextArea(YCUITagMeta* tagMeta);

	virtual ~YCUITextArea();

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