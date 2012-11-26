#include "YCIAction.h"

#include "YCIAnimation.h"

#include "YCBasic\YCAssert.h"

YCIAction::YCIAction(void)
	: myState(STOPPED)
	, myAnimation(NULL)
{
}

YCIAction::~YCIAction(void)
{
}

//
// 函数：start()
//
// 目的：启动动画
//
void YCIAction::start()
{
	myState = RUNNING;
}

//
// 函数：bind2Animation(YCIAnimation* animation)
//
// 目的：绑定动作到动画
//
void YCIAction::bind2Animation(YCIAnimation* animation)
{
	SMART_ASSERT(animation != NULL);

	myAnimation = animation;
}

//
// 函数：stop()
//
// 目的：停止动画
//
void YCIAction::stop()
{
	myState = STOPPED;
}

//
// 函数：isStopped()
//
// 目的：动画是否结束
//
bool YCIAction::isStopped()
{
	return (myState == STOPPED);
}