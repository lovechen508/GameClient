#include "YCParallelAction.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"

YCParallelAction::YCParallelAction(YCIAction* baseAction)
	: myParallelActions(new YCDList())
{
	SMART_ASSERT(baseAction != NULL);
}


YCParallelAction::~YCParallelAction(void)
{
	SAFE_DELETE(myParallelActions);
}

//
// 函数：run(unsigned int timestamp)
//
// 目的：跑动画
//
void YCParallelAction::run(unsigned int timestamp)
{


}
