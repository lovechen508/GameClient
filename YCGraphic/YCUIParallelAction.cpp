#include "YCUIParallelAction.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"

YCUIParallelAction::YCUIParallelAction(int durationTimes)
	: YCIUIAction(durationTimes)
	, myActions(new YCDList())
{
}

YCUIParallelAction::~YCUIParallelAction()
{
	delete myActions;
}
	
//
// 函数：addAction(YCIUIAction* action)
//
// 目的：添加Action到并行队列
//
void YCUIParallelAction::addAction(YCIUIAction* action)
{
	if (action == NULL)
	{
		throw YCException(2002, "YCUIParallelAction::addAction添加action为NULL");
	}
	myActions->append(action, NULL);
}

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
bool YCUIParallelAction::run()
{
	myDurationTimes -= 1;

	for (Item_List_T* item = myActions->begin();
		 item != myActions->end();
		 item = myActions->next(item))
	{
		YCIUIAction* action = (YCIUIAction*)myActions->payload(item);
		bool finished = action->run();
		if ( (myDurationTimes == 0) && !finished)
		{
			LOG_WARNING("YCUIParallelAction::run子动作速度不一致!");
		}
	}

	return (myDurationTimes == 0);
}