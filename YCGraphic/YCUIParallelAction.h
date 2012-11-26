#ifndef _INCLUDE_YCUIPARALLELACTION_H_
#define _INCLUDE_YCUIPARALLELACTION_H_

#include "YCIUIAction.h"

class YCDList;

/*
 * YCUIParallelAction : 并行执行的Action
 *
 * 说明：
 *
 */
class YCUIParallelAction : YCIUIAction
{
public:

	YCUIParallelAction(int durationTimes);

	virtual ~YCUIParallelAction();
	
	//
	// 函数：addAction(YCIUIAction* action)
	//
	// 目的：添加Action到并行队列
	//
	void addAction(YCIUIAction* action);

	//
	// 函数：bool run()
	//
	// 目的：由YCTimer触发调用run函数
	//
	// 注释：
	//       动作首先由front从ActionQueue中得到，如果
	//       动作执行完成返回true, 则从ActionQueue中pop出来
	//       动作执行完成返回false, 则仍然保留在ActionQueue中
	//
	virtual bool run();

private:
	
	YCDList * myActions;
};

#endif