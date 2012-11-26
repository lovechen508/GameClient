#ifndef _INCLUDE_YCPARALLELACTION_H_
#define _INCLUDE_YCPARALLELACTION_H_

#include "YCIAction.h"

class YCDList;

/*
 * YCParallelAction : Sprite并行动作类
 *
 *
 *
 *
 *
 */
class YCParallelAction :
	public YCIAction
{
public:

	//
	// 函数：YCParallelAction(YCIAction* baseAction)
	//
	// 目的：提供基础动作，构建并行动作
	//
	YCParallelAction(YCIAction* baseAction);

	virtual ~YCParallelAction(void);

	//
	// 函数：run(unsigned int timestamp)
	//
	// 目的：跑动画
	//
	void run(unsigned int timestamp);

	//
	// 函数：addAction(YCIAction* action)
	//
	// 目的：插入并行动作
	//
	void addAction(YCIAction* action);

	//
	// 函数：removeAction(YCIAction* action)
	//
	// 目的：移除并行动作
	//
	void removeAction(YCIAction* action);

private:

	//
	// 属性：myParallelActions
	//
	// 目的：并行动作列表
	//      按照先后顺序渲染
	//
	YCDList *myParallelActions;

};

#endif
