#ifndef _INCLUDE_YCUITR_H_
#define _INCLUDE_YCUITR_H_

#include "YCUITag.h"

class YCUITr :
	public YCUITag
{
public:
	YCUITr(YCUITagMeta* tagMeta);

	virtual ~YCUITr(void);

	//
	// ������layout(bool resizable)
	//
	// Ŀ�ģ�����Ԫ��������Ԫ���Ĳ���
	//
	virtual void layout(bool resizable);	

private:

	//
	// ������validateSubTag(const char* name)
	//
	// Ŀ�ģ�У���ӿؼ�
	//
	virtual bool validateSubTag(const char* name);
};

#endif

