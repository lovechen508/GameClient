#ifndef _INCLUDE_YCSINGLEACTION_H_
#define _INCLUDE_YCSINGLEACTION_H_

#include "YCIAction.h"

class YCSingleAction :
	public YCIAction
{
public:

	YCSingleAction(void);

	virtual ~YCSingleAction(void);

	//
	// 函数：run(unsigned int timestamp)
	//
	// 目的：跑动画
	//
	virtual void run(unsigned int timestamp);
};

#endif

