#ifndef _INCLUDE_YCTABLE_H_
#define _INCLUDE_YCTABLE_H_

#include "YCUITag.h"
#include "YCIScrollableTag.h"

class YCTable 
	: public YCUITag
	, YCIScrollableTag
{
public:

	YCTable(YCUITagMeta* tagMeta);

	virtual ~YCTable(void);
};

#endif

