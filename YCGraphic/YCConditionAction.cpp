#include "YCConditionAction.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"

YCConditionAction::YCConditionAction(YCIActionFunctor* condition)
	: myConditionActions(new YCDList())
	, myCondition(condition)
{
	SMART_ASSERT(condition != NULL);
}


YCConditionAction::~YCConditionAction(void)
{
	SAFE_DELETE(myCondition);
	SAFE_DELETE(myConditionActions);
}

//
// 函数：addBranch(YCIAction* action)
//
// 目的：添加分支
//
void YCConditionAction::addBranch(YCIAction* action)
{
	myConditionActions->append(action, NULL);
}
