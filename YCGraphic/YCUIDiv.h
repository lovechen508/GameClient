#ifndef _INCLUDE_YCUIDIV_H_
#define _INCLUDE_YCUIDIV_H_

#include "YCUITag.h"
#include "YCIScrollableTag.h"

class YCUIDiv 
	: public YCUITag
	, public YCIScrollableTag
{
public:

	YCUIDiv(YCUITagMeta* tagMeta);

	virtual ~YCUIDiv(void);

	//
	// ������draw()
	//
	// Ŀ�ģ���ָ�������Ƹ�UIԪ��
	//
	//void draw();
	
private:

	//
	// ������validateSubTag(const char* name)
	//
	// Ŀ�ģ�У���ӿؼ�
	//
	virtual bool validateSubTag(const char* name);

};

#endif

