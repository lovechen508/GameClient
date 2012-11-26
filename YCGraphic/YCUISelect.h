#ifndef _INCLUDE_YCUISELECT_H_
#define _INCLUDE_YCUISELECT_H_

#include "YCUITag.h"

/*
 * YCSelect :  ����ʽ�˵�
 */
class YCUISelect 
	: public YCUITag
{
public:

	YCUISelect(YCUITagMeta* tagMeta);
	
	virtual ~YCUISelect(void);

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