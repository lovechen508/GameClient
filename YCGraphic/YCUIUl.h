#ifndef _INCLUDE_YCUIUL_H_
#define _INCLUDE_YCUIUL_H_

#include "YCUITag.h"
#include "YCIScrollableTag.h"

class YCUIUl 
	: public YCUITag
	, public YCIScrollableTag
{
public:

	YCUIUl(YCUITagMeta* tagMeta);

	virtual ~YCUIUl(void);

private:

	//
	// ������validateSubTag(const char* name)
	//
	// Ŀ�ģ�У���ӿؼ�
	//
	virtual bool validateSubTag(const char* name);
};

#endif
