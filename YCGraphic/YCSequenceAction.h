#ifndef _INCLUDE_YCSEQUENCEACTION_H_
#define _INCLUDE_YCSEQUENCEACTION_H_

#include "YCIAction.h"

/*
 * YCSequenceAction : 顺序类型动画
 *
 *
 */
class YCSequenceAction :
	public YCIAction
{
public:

	YCSequenceAction(void);

	virtual ~YCSequenceAction(void);

	//
	// 函数：run(unsigned int timestamp)
	//
	// 目的：跑动画
	//
	virtual void run(unsigned int timestamp);
};

#endif
