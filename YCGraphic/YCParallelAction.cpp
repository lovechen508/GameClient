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
// ������run(unsigned int timestamp)
//
// Ŀ�ģ��ܶ���
//
void YCParallelAction::run(unsigned int timestamp)
{


}
