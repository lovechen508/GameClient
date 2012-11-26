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
// ������start()
//
// Ŀ�ģ���������
//
void YCIAction::start()
{
	myState = RUNNING;
}

//
// ������bind2Animation(YCIAnimation* animation)
//
// Ŀ�ģ��󶨶���������
//
void YCIAction::bind2Animation(YCIAnimation* animation)
{
	SMART_ASSERT(animation != NULL);

	myAnimation = animation;
}

//
// ������stop()
//
// Ŀ�ģ�ֹͣ����
//
void YCIAction::stop()
{
	myState = STOPPED;
}

//
// ������isStopped()
//
// Ŀ�ģ������Ƿ����
//
bool YCIAction::isStopped()
{
	return (myState == STOPPED);
}