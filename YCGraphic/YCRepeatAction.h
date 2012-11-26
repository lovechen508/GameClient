#ifndef _INCLUDE_YCREPEATACTION_H_
#define _INCLUDE_YCREPEATACTION_H_

#include "YCIAction.h"

/*
 * YCRepeatAction : 重复循环类动画, 直到被外部条件中断
 *
 *
 */
class YCRepeatAction :	public YCIAction
{
public:

	YCRepeatAction(void);
	
	virtual ~YCRepeatAction(void);

	//
	// 函数：run(unsigned int timestamp)
	//
	// 目的：跑动画
	//
	virtual void run(unsigned int timestamp);

};

#endif

