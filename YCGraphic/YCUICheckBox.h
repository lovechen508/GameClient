#ifndef _INCLUDE_YCUICHECKBOX_H_
#define _INCLUDE_YCUICHECKBOX_H_

#include "YCIUIToggleButton.h"

/*
 * YCCheckBox : 
 */
class YCUICheckBox
	: public YCIUIToggleButton
{
public:

	YCUICheckBox(YCUITagMeta* tagMeta);

	virtual ~YCUICheckBox();

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