#ifndef _INCLUDE_YCIUITOGGLEBUTTON_H_
#define _INCLUDE_YCIUITOGGLEBUTTON_H_

#include "YCUITag.h"

/*
 * YCIToggleButton : 两态开关基类
 */
class YCIUIToggleButton
	: public YCUITag
{
public:


protected:

	YCIUIToggleButton(YCUITagMeta* tagMeta);

	virtual ~YCIUIToggleButton();
};

#endif