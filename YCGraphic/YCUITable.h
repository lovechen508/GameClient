#ifndef _INCLUDE_YCUITABLE_H_
#define _INCLUDE_YCUITABLE_H_

#include "YCUITag.h"
#include "YCIScrollableTag.h"

class YCUITable 
	: public YCUITag
	, public YCIScrollableTag
{
public:

	YCUITable(YCUITagMeta* tagMeta);

	virtual ~YCUITable(void);

	//
	// ������layout(bool resizable)
	//
	// Ŀ�ģ�����Ԫ��������Ԫ���Ĳ���
	//
	virtual void layout(bool resizable);

protected:

	//
	// ������extraDraw()
	//
	// Ŀ�ģ�UIԪ�����Զ������
	//
	virtual void extraDraw();

	//
	// ������extraCSS()
	//
	// Ŀ�ģ������ǩ�ض���CSS����
	//
	virtual void extraCSS();

private:

	//
	// ������validateSubTag(const char* name)
	//
	// Ŀ�ģ�У���ӿؼ�
	//
	virtual bool validateSubTag(const char* name);
};

#endif

